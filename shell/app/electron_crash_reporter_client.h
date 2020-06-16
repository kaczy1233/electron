// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_
#define SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_

#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/no_destructor.h"
#include "build/build_config.h"
#include "components/crash/core/app/crash_reporter_client.h"  // nogncheck

class ElectronCrashReporterClient : public crash_reporter::CrashReporterClient {
 public:
  static void Create();

  static ElectronCrashReporterClient* Get();
  void SetCollectStatsConsent(bool upload_allowed);
  void SetUploadUrl(const std::string& url);
  void SetShouldRateLimit(bool rate_limit);
  void SetShouldCompressUploads(bool compress_uploads);
  void SetGlobalAnnotations(
      const std::map<std::string, std::string>& annotations);

  // crash_reporter::CrashReporterClient implementation.
#if defined(OS_LINUX)
  void SetCrashReporterClientIdFromGUID(
      const std::string& client_guid) override;
  void GetProductNameAndVersion(const char** product_name,
                                const char** version) override;
  void GetProductNameAndVersion(std::string* product_name,
                                std::string* version,
                                std::string* channel) override;
  base::FilePath GetReporterLogFilename() override;
#endif

#if defined(OS_WIN)
  void GetProductNameAndVersion(const base::string16& exe_path,
                                base::string16* product_name,
                                base::string16* version,
                                base::string16* special_build,
                                base::string16* channel_name) override;
#endif

#if defined(OS_WIN)
  bool GetCrashDumpLocation(base::string16* crash_dir) override;
#else
  bool GetCrashDumpLocation(base::FilePath* crash_dir) override;
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
  bool GetCrashMetricsLocation(base::FilePath* metrics_dir) override;
#endif

  bool IsRunningUnattended() override;

  bool GetCollectStatsConsent() override;

  bool GetShouldRateLimit() override;
  bool GetShouldCompressUploads() override;

  void GetProcessSimpleAnnotations(
      std::map<std::string, std::string>* annotations) override;

#if defined(OS_MACOSX)
  bool ReportingIsEnforcedByPolicy(bool* breakpad_enabled) override;
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
  bool ShouldMonitorCrashHandlerExpensively() override;
#endif

  bool EnableBreakpadForProcess(const std::string& process_type) override;

  std::string GetUploadUrl() override;

 private:
  friend class base::NoDestructor<ElectronCrashReporterClient>;

  std::string upload_url_;
  bool collect_stats_consent_;
  bool rate_limit_ = false;
  bool compress_uploads_ = false;
  std::map<std::string, std::string> global_annotations_;

  ElectronCrashReporterClient();
  ~ElectronCrashReporterClient() override;

  DISALLOW_COPY_AND_ASSIGN(ElectronCrashReporterClient);
};

#endif  // SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_
