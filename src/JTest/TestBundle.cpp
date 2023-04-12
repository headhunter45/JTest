#include <JTest/TestBundle.h>
#include <algorithm>
#include <vector>
#include <JTest/DescribeOptions.h>
#include <JTest/Test.h>
#include <string>
#include <optional>
#include <iostream>

namespace JTest {
    using std::string;
    using std::vector;
    using std::nullopt;

    TestBundle::TestBundle(const vector<TestBundle>& bundles, const DescribeOptions& options)
    : label_("")
    , fully_qualified_path_("")
    , tests_({})
    , children_({})
    , afterAll_(options.getAfterAll())
    , afterEach_(options.getAfterEach())
    , beforeAll_(options.getBeforeAll())
    , beforeEach_(options.getBeforeEach())
    , disabled_(false) {
        for_each(bundles.begin(), bundles.end(), [this](TestBundle bundle) {
            if (bundle.tests_.size() == 0 && bundle.children_.size() == 1 && bundle.label_.size() == 0) {
                tests_.push_back(bundle.tests_.at(0));
            } else {
                children_.push_back(bundle);
            }
        });
    }

    TestBundle::TestBundle(const string& label, const vector<Test>& tests)
    : label_(label)
    , fully_qualified_path_("")
    , tests_({})
    , children_({})
    , afterAll_(nullopt)
    , afterEach_(nullopt)
    , beforeAll_(nullopt)
    , beforeEach_(nullopt)
    , disabled_(false) {
        for_each(tests.begin(), tests.end(), [this](Test test) {
            tests_.push_back(test);
        });
    }

    string TestBundle::fully_qualified_label() const {
        return (fully_qualified_path_.length()>0?fully_qualified_path_ + "::":"") + (label_.length()>0?label_:"<anonymous>");
    }
}
