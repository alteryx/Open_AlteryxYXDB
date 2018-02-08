#define _WINSOCKAPI_
#pragma warning( push, 0 )
#include "../stdafx.h"
#pragma warning( pop )

#include "n-api-sandbox.h"

#pragma warning( push, 0 )
#include <napi-thread-safe-callback.hpp>
#pragma warning( pop )

#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::exception;
using std::move;
using std::thread;
using std::vector;

namespace std { namespace chrono { } namespace this_thread { } }

namespace chrono = std::chrono;
namespace this_thread = std::this_thread;

using namespace SRC;


#define DO_LOGGING(x) \
{ \
	std::stringstream s; \
	s << x << '\n'; \
	std::cout << s.str(); \
}

//void
//NApiSandbox::Initialize(Napi::Env &env, Napi::Object &target) {
//	Napi::Function constructor = DefineClass(env, "NApiSandbox", {
//		InstanceMethod("greet", &NApiSandbox::Greet)
//	});
//
//	target.Set("NApiSandbox", constructor);
//}
//
//#define CHECK_STATUS(status, env, msg) \
//{ \
//	if (status != napi_ok) \
//	{ \
//		throw Napi::Error::New(env, msg); \
//	} \
//}
//
//#ifndef CALLBACKMETHOD
//void
//NApiSandbox::Greet(const Napi::CallbackInfo &info)
//{
//	DO_LOGGING("Greeting on thread " << this_thread::get_id());
//	if (!info[0].IsString())
//	{
//		throw Napi::Error::New(info.Env(), "You need to introduce yourself to greet");
//	}
//	if (!info[1].IsFunction())
//	{
//		throw Napi::Error::New(info.Env(), "internal error in javascript binding; a callback must be given for resolve");
//	}
//	if (!info[2].IsFunction())
//	{
//		throw Napi::Error::New(info.Env(), "internal error in javascript binding; a callback must be given for reject");
//	}
//
//	auto name{ info[0].As<Napi::String>() };
//
//	printf("Hello %s\n", name.Utf8Value().c_str());
//	printf("I am %s\n", this->_greeterName.Value().ToString().Utf8Value().c_str());
//
//	//auto scope{ Napi::HandleScope{ info.Env() } };
//
//	auto rsv = info[1].As<Napi::Function>();
//	DO_LOGGING("creating resolve and reject");
//
//	auto resolve{ ThreadSafeCallback(move(rsv)) };
//	DO_LOGGING("resolve created");
//	auto reject{ ThreadSafeCallback(info[2].As<Napi::Function>()) };
//	DO_LOGGING("resolve and reject created");
//
//	executor = thread{ [/*scope = move(scope),*/ resolve = move(resolve), reject = move(reject), gn = _greeterName.Value().ToString().Utf8Value()]() mutable
//	{
//		try
//		{
//			DO_LOGGING("entering thread " << this_thread::get_id());
//			this_thread::sleep_for(chrono::seconds{ 5 });
//			DO_LOGGING("woke up");
//
//			resolve.call([gn](Napi::Env env, vector<napi_value>& values)
//			{
//				DO_LOGGING("resolving on thread " << this_thread::get_id());
//				values = { Napi::String::New(env, gn) };
//				DO_LOGGING("resolved on thread " << this_thread::get_id());
//			});
//		}
//		catch(exception e)
//		{
//			DO_LOGGING("caught " << e.what());
//		}
//		catch(...)
//		{
//			DO_LOGGING("caught an exception in other thread...");
//
//			try
//			{
//				reject.call([](Napi::Env env, vector<napi_value>& values)
//				{
//					DO_LOGGING("rejecting on thread " << this_thread::get_id());
//					values = { env.Undefined() };
//					DO_LOGGING("rejecting on thread " << this_thread::get_id());
//				});
//			}
//			catch(...)
//			{
//				DO_LOGGING("caught an exception attempting to reject...");
//			}
//		}
//	} };
//}
//#endif
//
//#ifdef CALLBACKMETHOD
//void
//NApiSandbox::Greet(const Napi::CallbackInfo &info) {
//	if (!info[0].IsString())
//	{
//		throw Napi::Error::New(info.Env(), "You need to introduce yourself to greet");
//	}
//	if (!info[1].IsFunction())
//	{
//		throw Napi::Error::New(info.Env(), "internal error in javascript binding; a callback must be given for resolve");
//	}
//	if (!info[2].IsFunction())
//	{
//		throw Napi::Error::New(info.Env(), "internal error in javascript binding; a callback must be given for reject");
//	}
//
//	auto name{ info[0].As<Napi::String>() };
//	printf("Hello %s\n", name.Utf8Value().c_str());
//	printf("I am %s\n", this->_greeterName.Value().ToString().Utf8Value().c_str());
//
//	auto scope{ Napi::HandleScope{ info.Env() } };
//
//	auto resolve{ Persistent(info[1].ToObject()) };
//
//	auto reject{ Persistent(info[2].ToObject()) };
//
//	_greeterName.Ref();
//	auto greeter_name{ _greeterName.Value() };
//
//	executor = thread{
//	//	[]()
//	//{
//	//	DO_LOGGING("launched a thread");
//	//	this_thread::sleep_for(chrono::seconds(5));
//	//	DO_LOGGING("finished other thread");
//	//} };
//		[
//			scope = move(scope)
//			, resolve = move(resolve)
//			, reject = move(reject)
//			, greeter_name = move(greeter_name)
//		]
//	{
//		try
//		{
//			const auto resource_name{ Napi::String::New(scope.Env(), "async promise") };
//			auto async_context{ napi_async_context{} };
//			{
//				auto status{ napi_async_init(scope.Env(), nullptr, resource_name, &async_context) };
//				if (status != napi_ok) throw Napi::Error::New(scope.Env(), "error initializing async context");
//			}
//
//			auto cleanup_context = gsl::finally([&scope, async_context]
//			{
//				try
//				{
//					auto status{ napi_async_destroy(scope.Env(), async_context) };
//					if (status != napi_ok)
//					{
//						DO_LOGGING("error destroying async context");
//					}
//					else
//					{
//						DO_LOGGING("successfully destroyed async context");
//					}
//				}
//				catch (...)
//				{
//					DO_LOGGING("caught an exception while trying to destroy async context");
//				}
//			});
//
//			DO_LOGGING("entered processing thread... " << scope.Env().IsExceptionPending());
//			//this_thread::sleep_for(chrono::seconds(12));
//			//auto name = greeter_name.Value().ToString().Utf8Value();
//			//DO_LOGGING("done sleeping in processing thread for " << name << " " << scope.Env().IsExceptionPending() << "...");
//
//			DO_LOGGING("attempting to retrieve greeter name");
//			auto pv_name{ static_cast<napi_value>(greeter_name) };
//			DO_LOGGING("retrieved greeter name");
//			auto result{ napi_value{} };
//			DO_LOGGING("attempting to make resolve callback");
//			auto local_resolve = resolve.Value();
//			DO_LOGGING("made local_resolve");
//			auto status{ napi_make_callback(scope.Env(), async_context, local_resolve, local_resolve, 1, &pv_name, &result) };
//			if (status != napi_ok)
//			{
//				DO_LOGGING("could not make callback to set value");
//			}
//			else
//			{
//				DO_LOGGING("made callback to set value");
//			}
//		}
//		catch (Napi::Error e)
//		{
//			DO_LOGGING(e.Message());
//			try
//			{
//				//auto error{ static_cast<napi_value>(e.Value()) };
//				//auto result{ napi_value{} };
//				//auto status{ napi_make_callback(scope.Env(), async_context, reject.Value(), reject.Value(), 1, &error, &result) };
//				//if (status != napi_ok)
//				//{
//				//	DO_LOGGING("failed to reject promise");
//				//}
//				//else
//				//{
//				//	DO_LOGGING("rejected promise");
//				//}
//			}
//			catch (...)
//			{
//				DO_LOGGING("caught an exception while attempting to reject JS promise!");
//			}
//		}
//		catch (exception e)
//		{
//			DO_LOGGING(e.what());
//		}
//		catch (...)
//		{
//			DO_LOGGING("caught an exception!");
//			try
//			{
//				//auto error{ napi_value{} };
//				//auto result{ napi_value{} };
//				//DO_LOGGING("attempting to make reject callback");
//				//auto status{ napi_make_callback(scope.Env(), async_context, reject.Value(), reject.Value(), 1, &error, &result) };
//				//if (status != napi_ok)
//				//{
//				//	DO_LOGGING("failed to reject promise");
//				//}
//				//else
//				//{
//				//	DO_LOGGING("rejected promise");
//				//}
//			}
//			catch (...)
//			{
//				DO_LOGGING("caught an exception while attempting to reject JS promise!");
//			}
//		}
//		//}();
//	} };
//}
//#endif
//
//NApiSandbox::NApiSandbox(const Napi::CallbackInfo &info)
//	: Napi::ObjectWrap<NApiSandbox>{info}
//{
//	DO_LOGGING("Constructing NApiSandbox on thread " << this_thread::get_id());
//	if (!info[0].IsString())
//	{
//		throw Napi::Error::New(info.Env(), "You must name me");
//	}
//
//	this->_greeterName = Persistent(info[0].ToObject());
//}
//
//NApiSandbox::~NApiSandbox()
//{
//	DO_LOGGING("calling ~NApiSandbox" << this_thread::get_id());
//	if (executor.joinable())
//	{
//		executor.join();
//	}
//}

namespace node {

void
RecordInfo::Initialize(Napi::Env &env, Napi::Object &target) {
	Napi::Function constructor = DefineClass(env, "RecordInfo", {
		InstanceMethod("add_field", &RecordInfo::AddField)
		, InstanceMethod("get_record_xml_meta_data", &RecordInfo::GetRecordXmlMetaData)
	});

	target.Set("RecordInfo", constructor);
}

RecordInfo::RecordInfo(const Napi::CallbackInfo &info)
	: Napi::ObjectWrap<RecordInfo>{info}
{
}

RecordInfo::~RecordInfo()
{
}

void
RecordInfo::AddField(const Napi::CallbackInfo &info)
{
	if (!info[0].IsString())
	{
		throw Napi::Error::New(info.Env(), "name must be passed");
	}
	if (!info[1].IsNumber())
	{
		throw Napi::Error::New(info.Env(), "type must be passed");
	}

	const auto name{ info[0].As<Napi::String>() };
	const auto type{ info[1].As<Napi::Number>() };

	m_recordInfo.AddField(SRC::RecordInfo::CreateFieldXml(ConvertToWString(name.Utf8Value().c_str()), static_cast<E_FieldType>(type.Int32Value())));
}

Napi::Value
RecordInfo::GetRecordXmlMetaData(const Napi::CallbackInfo &info)
{
	return Napi::String::New(info.Env(), ConvertToAString(m_recordInfo.GetRecordXmlMetaData()).c_str());
}

}

Napi::Object
Init(Napi::Env env, Napi::Object exports) {
	//NApiSandbox::Initialize(env, exports);
	node::RecordInfo::Initialize(env, exports);

	return exports;
}

NODE_API_MODULE(addon, Init);