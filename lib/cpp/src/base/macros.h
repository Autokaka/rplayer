/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2022/05/01.
 */

#pragma once

#define DISALLOW_COPY(TypeName) TypeName(const TypeName&) = delete

#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete

#define DISALLOW_MOVE(TypeName)  \
  TypeName(TypeName&&) = delete; \
  TypeName& operator=(TypeName&&) = delete

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  TypeName& operator=(const TypeName&) = delete

#define DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName)  \
  TypeName(const TypeName&) = delete;            \
  TypeName(TypeName&&) = delete;                 \
  TypeName& operator=(const TypeName&) = delete; \
  TypeName& operator=(TypeName&&) = delete

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                           \
  DISALLOW_COPY_ASSIGN_AND_MOVE(TypeName)
