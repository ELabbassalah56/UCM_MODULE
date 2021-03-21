Introduction {#introduction}
============================

This document describes the design (approach and decisions) of the Functional Cluster Update and Configuration Management for AUTOSAR's Adaptive Platform Demonstrator.

The decisions taken for the AUTOSAR Adaptive Platform Demonstrator may not apply other implementations as the standard is defined by the specifications. Nevertheless, the Demonstrator may supplement and ease the understanding of the specifications.

This implementation of Update and Configuration Management software provides a complete and well-documented reference implementation that is meant to be used in prototype projects as well as a basis for production-grade implementations. The main goal here was to provide proof of concepts for Update and Configuration Management features specified in the Update and Configuration Management specification of AUTOSAR Adaptive. Optimization for memory consumption and speed have been considered but they
haven't played a major role in overall software design and implementation. More than that, no static or dynamic code analysis has been performed, therefore any series production project deriving form this implementation, will have to further fulfill the safety constraints described in the industry standards.

# Known limitations {#known_limitations}

Limitations of the Demonstrator implementation of the Functional Cluster.

- Not all of the SWS requirements are implemented and they are listed here:

| Feature                            | Requirement ID       | Implementation status  |
|------------------------------------|----------------------|------------------------|
| ucm_Software Package               | SWS_UCM_00122        | Partially implemented. |
| ucm_Content of Software Package    | SWS_UCM_00112        | Not implemented.       |
| ucm_Content of Software Package    | SWS_UCM_CONSTR_00001 | Not implemented.       |
| ucm_Content of Software Package    | SWS_UCM_00130        | Not implemented.       |
| ucm_Applications Persisted Data    | SWS_UCM_00011        | Not implemented.       |
| ucm_Applications Persisted Data    | SWS_UCM_00113        | Not implemented.       |
|                                    | SWS_UCM_00120        | Not implemented.       |
|                                    | SWS_UCM_00128        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00007        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00088        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00140        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00008        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00145        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00010        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00156        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00087        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00092        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00028        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00098        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00091        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00075        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00141        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00021        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00093        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00148        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00069        | Partially implemented. |
| ucm_Transferring Software Packages | SWS_UCM_00165        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00166        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00167        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00168        | Not implemented.       |
| ucm_Transferring Software Packages | SWS_UCM_00169        | Not implemented.       |
| ucm_Processing Software Packages   | SWS_UCM_00001        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00137        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00018        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00029        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00104        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00003        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00024        | Partially implemented. |
| ucm_Processing Software Packages   | SWS_UCM_00161        | Not implemented.       |
|                                    | SWS_UCM_00019        | Partially implemented. |
|                                    | SWS_UCM_00086        | Partially implemented. |
|                                    | SWS_UCM_00080        | Partially implemented. |
|                                    | SWS_UCM_00147        | Partially implemented. |
|                                    | SWS_UCM_00082        | Partially implemented. |
|                                    | SWS_UCM_00150        | Partially implemented. |
|                                    | SWS_UCM_00149        | Partially implemented. |
|                                    | SWS_UCM_00151        | Partially implemented. |
|                                    | SWS_UCM_00081        | Partially implemented. |
|                                    | SWS_UCM_00017        | Partially implemented. |
|                                    | SWS_UCM_00083        | Partially implemented. |
|                                    | SWS_UCM_00152        | Partially implemented. |
|                                    | SWS_UCM_00084        | Partially implemented. |
|                                    | SWS_UCM_00153        | Partially implemented. |
|                                    | SWS_UCM_00154        | Partially implemented. |
|                                    | SWS_UCM_00085        | Partially implemented. |
|                                    | SWS_UCM_00126        | Partially implemented. |
|                                    | SWS_UCM_00155        | Partially implemented. |
|                                    | SWS_UCM_00111        | Partially implemented. |
|                                    | SWS_UCM_00146        | Partially implemented. |
|                                    | SWS_UCM_00162        | Partially implemented. |
|                                    | SWS_UCM_00163        | Partially implemented. |
|                                    | SWS_UCM_00164        | Partially implemented. |
|                                    | SWS_UCM_00127        | Partially implemented. |
|                                    | SWS_UCM_00108        | Not implemented.       |
|                                    | SWS_UCM_00107        | Partially implemented. |
|                                    | SWS_UCM_00026        | Not implemented.       |
|                                    | SWS_UCM_00027        | Not implemented.       |
|                                    | SWS_UCM_00025        | Partially implemented. |
|                                    | SWS_UCM_00022        | Partially implemented. |
|                                    | SWS_UCM_00005        | Partially implemented. |
|                                    | SWS_UCM_00110        | Partially implemented. |
|                                    | SWS_UCM_00142        | Not implemented.       |
|                                    | SWS_UCM_00094        | Partially implemented. |
|                                    | SWS_UCM_00096        | Partially implemented. |
|                                    | SWS_UCM_00020        | Partially implemented. |
|                                    | SWS_UCM_00157        | Not implemented.       |
|                                    | SWS_UCM_00158        | Not implemented.       |
|                                    | SWS_UCM_00170        | Partially implemented. |
|                                    | SWS_UCM_00143        | Partially implemented. |
|                                    | SWS_UCM_00171        | Partially implemented. |
|                                    | SWS_UCM_00172        | Partially implemented. |
|                                    | SWS_UCM_00115        | Partially implemented. |
|                                    | SWS_UCM_00160        | Not implemented.       |
|                                    | SWS_UCM_00004        | Partially implemented. |
|                                    | SWS_UCM_00030        | Partially implemented. |
|                                    | SWS_UCM_00103        | Partially implemented. |
|                                    | SWS_UCM_00105        | Partially implemented. |
|                                    | SWS_UCM_01***        | Not implemented.       |
| ucm_Service interfaces             | SWS_UCM_00073        | Partially implemented. |
| ucm_Service interfaces             | SWS_UCM_00131        | Partially implemented. |
