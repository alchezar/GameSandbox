**Code name:** PetProject №6  
**Project name:** Action Roguelike  
**Instructor:** Tom Looman  
**Price:** ~~$350~~ $0  
**Links:** 
[Web page](https://courses.tomlooman.com/p/unrealengine-cpp), 
[Notion](https://www.notion.so/Professional-Game-Development-in-C-and-Unreal-Engine-c0fc83a99b124119ae2ce9abfc265ef7)  
**Comments:**  
- Initialize components binding is better in  `ThisClass::PostInitializeComponents()` instead default constructor or `BeginPlay()`
- Instead of using `printstring` it is better to display information on screen not only in top left corner, but in any point of the screen by `DrawDebugString(GetWorld(), $Location$, FString::Printf(TEXT(”Hello”), nullptr, FColor::Green, 2.f);`