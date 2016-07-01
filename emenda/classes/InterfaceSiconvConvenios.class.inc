<?php
class loginWS {
	public $id; // long
	public $senha; // string
	public $usuario; // string
}

class enviarNotaEmpenho {
	public $arg0; // convenioWS
}

class convenioWS {
	public $ano; // int
	public $dataAssinatura; // dateTime
	public $dataPrestacaoContas; // dateTime
	public $dataPublicacao; // dateTime
	public $empenhado; // boolean
	public $fimExecucao; // dateTime
	public $fundamentoLegal; // string
	public $id; // long
	public $idHash; // string
	public $inadimplente; // boolean
	public $inicioExecucao; // dateTime
	public $justificativa; // string
	public $justificativaAlteracaoAdimplencia; // string
	public $login; // loginWS
	public $notaEmpenhoWS; // notaEmpenhoWS
	public $notaLancamentoWS; // notaLancamentoWS
	public $numeroControleExterno; // string
	public $numeroInterno; // string
	public $numeroProcesso; // string
	public $permiteAjustesCronFisico; // boolean
	public $prazoPrestacaoContas; // int
	public $prazoProrrogacaoPrestacaoContas; // int
	public $propostaConvenioWS; // propostaConvenioWS
	public $propostaWS; // propostaWS
	public $prorrogasOficio; // prorrogaOficioWS
	public $publicado; // boolean
	public $sequencial; // int
	public $situacaoPublicacaoWS; // string
	public $statusConvenioWS; // string
	public $termosAditivosWS; // termoAditivoWS
	public $tipoBloqueioWS; // string
	public $tipoConvenioWS; // string
}

class notaEmpenhoWS {
	public $amparoLegal; // string
	public $ano; // string
	public $codigoErroSiafi; // string
	public $cronogramaEmpenhoWS; // cronogramaEmpenhoWS
	public $dataEmissao; // dateTime
	public $dataEnvioXml; // dateTime
	public $dataPagamento; // dateTime
	public $erroSiafi; // string
	public $esferaOrcamentaria; // string
	public $especieEmpenho; // tipoNotaEmpenhoWS
	public $favorecido; // string
	public $fonteRecurso; // string
	public $gestaoEmitente; // string
	public $gestaoEmpenhoOriginal; // string
	public $gestaoFavorecida; // string
	public $gestaoReferencia; // string
	public $id; // long
	public $idHash; // string
	public $inciso; // string
	public $indicadorContraEntregaWS; // string
	public $indicadorResultado; // string
	public $indicadorTipoCredito; // string
	public $itensEmpenhoWS; // itemEmpenhoWS
	public $modalidadeLicitacao; // long
	public $municipioBeneficiado; // string
	public $numero; // string
	public $numeroConvenio; // string
	public $numeroEmpenhoOriginal; // string
	public $numeroInterno; // long
	public $numeroInternoConcedente; // string
	public $numeroLista; // string
	public $objeto; // string
	public $observacao; // string
	public $origemMaterial; // string
	public $planoInterno; // string
	public $programaDeTrabalho; // string
	public $ptres; // string
	public $situacaoWS; // string
	public $taxaCambio; // double
	public $tipoEmpenhoWS; // tipoEmpenhoWS
	public $ugEmitente; // string
	public $ugEmpenhoOriginal; // string
	public $ugReferencia; // string
	public $ugr; // string
	public $unidadeFederativaWS; // unidadeFederativaWS
	public $unidadeOrcamentaria; // string
	public $valorEmpenho; // double
}

class cronogramaEmpenhoWS {
	public $dataRecebimento; // dateTime
	public $dataVencimento; // dateTime
	public $id; // long
	public $idHash; // string
	public $valor; // double
}

class tipoNotaEmpenhoWS {
	public $codigo; // string
	public $id; // long
}

class itemEmpenhoWS {
	public $descricao; // string
	public $especie; // int
	public $id; // long
	public $idHash; // string
	public $naturezaDespesaSubItem; // string
	public $quantidade; // double
	public $referencia; // string
	public $valorUnitario; // double
}

class tipoEmpenhoWS {
	public $codigo; // string
	public $id; // long
	public $idHash; // string
}

class enviarNotaLancamento {
	public $arg0; // notaLancamentoEnvioWS
}

class notaLancamentoEnvioWS {
	public $id; // long
	public $idHash; // string
	public $login; // loginWS
	public $notasLancamento; // notaLancamentoWS
}

class notaLancamentoWS {
	public $ano; // string
	public $codErroSiafi; // string
	public $dataEmissao; // dateTime
	public $dataEnvioXml; // dateTime
	public $dataValorizacao; // dateTime
	public $dataVencimentoTituloCredito; // dateTime
	public $erroSiafi; // string
	public $eventoNotaLancamentoWS; // eventoNotaLancamentoWS
	public $favorecido; // string
	public $gestaoEmitente; // string
	public $gestaoFavorecida; // string
	public $id; // long
	public $idHash; // string
	public $inverteSaldo; // string
	public $numConvenio; // string
	public $numeroControleExterno; // string
	public $numeroDocumento; // string
	public $numeroInterno; // long
	public $numeroInternoConcedente; // string
	public $observacao; // string
	public $selecionadoDh; // string
	public $situacao; // string
	public $taxaCambio; // double
	public $tituloCredito; // string
	public $uasgEmitente; // string
	public $ugEmitente; // string
}

class eventoNotaLancamentoWS {
	public $classificacao1; // string
	public $classificacao2; // string
	public $id; // long
	public $idHash; // string
	public $inscricao1; // string
	public $inscricao2; // string
	public $numero; // string
	public $valorLancamento; // double
}

class enviarNotaLancamentoResponse {
	public $return; // string
}

class atualizarCronogramaDesembolso {
	public $arg0; // parametrosAtualizarCronogramaDesembolsoWS
}

class parametrosAtualizarCronogramaDesembolsoWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $parcelas; // parametrosParcelaCronogramaDesembolsoWS
	public $sequencialConvenio; // int
}

class parametrosParcelaCronogramaDesembolsoWS {
	public $ano; // int
	public $mes; // int
	public $parcelasMetas; // parametrosParcelaMetaWS
	public $responsavel; // string
	public $valor; // decimal
}

class parametrosParcelaMetaWS {
	public $especificacaoMeta; // string
	public $parcelasEtapas; // parametrosParcelaEtapaWS
	public $valor; // decimal
}

class parametrosParcelaEtapaWS {
	public $especificacaoEtapa; // string
	public $valor; // decimal
}

class atualizarCronogramaDesembolsoResponse {
	public $return; // string
}

class atualizarPareceresPlanoTrabalho {
	public $arg0; // parametrosAtualizarPareceresPlanoTrabalhoWS
}

class parametrosAtualizarPareceresPlanoTrabalhoWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $pareceresPlanoTrabalho; // parametrosParecerPlanoTrabalhoWS
	public $sequencialConvenio; // int
}

class parametrosParecerPlanoTrabalhoWS {
	public $atribuicaoResponsavel; // string
	public $cpfResponsavel; // string
	public $data; // dateTime
	public $funcaoResponsavel; // string
	public $parecer; // string
	public $situacao; // string
}

class atualizarPareceresPlanoTrabalhoResponse {
	public $return; // string
}

class enviarOrdemBancaria {
	public $arg0; // ordemBancariaEnvioWS
}

class ordemBancariaEnvioWS {
	public $id; // long
	public $login; // loginWS
	public $ordensBancarias; // ordemBancariaWS
}

class ordemBancariaWS {
	public $ano; // int
	public $anoParcelaCronoDesembolso; // long
	public $codigoAgenciaOrigem; // string
	public $codigoBancoOrigem; // string
	public $controleSTNOriginal; // string
	public $data; // dateTime
	public $eventoOBWS; // eventoOrdemBancariaWS
	public $gestaoDocumentoOrigem; // string
	public $gestaoEmitente; // string
	public $gestaoUgFavorecida; // string
	public $id; // long
	public $idHash; // string
	public $idTransferencia; // long
	public $inverteSaldo; // string
	public $mesParcelaCronoDesembolso; // long
	public $numContaCorrenteOrigem; // string
	public $numConvenio; // string
	public $numDocumentoOrigem; // string
	public $numListaOB; // long
	public $numProcesso; // long
	public $numeroControleExterno; // string
	public $numeroInterno; // string
	public $numeroInternoConcedente; // string
	public $observacao; // string
	public $taxaCambio; // double
	public $ugDocumentoOrigem; // string
	public $ugEmitente; // string
	public $valorTotalOB; // double
}

class eventoOrdemBancariaWS {
	public $classificacao1; // string
	public $classificacao2; // string
	public $codEvento; // string
	public $id; // long
	public $idHash; // string
	public $inscricao1; // string
	public $inscricao2; // string
	public $valorLancamento; // double
}

class enviarOrdemBancariaResponse {
	public $return; // string
}

class atualizarProgramas {
	public $arg0; // parametrosAtualizarProgramasWS
}

class parametrosAtualizarProgramasWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $programas; // parametrosProgramaWS
	public $sequencialConvenio; // int
}

class parametrosProgramaWS {
	public $codigoPrograma; // string
	public $objetos; // parametrosObjetoProgramaWS
	public $regraContrapartida; // string
	public $valorBeneficiarioEspecifico; // parametrosValorBeneficiarioEspecificoWS
	public $valorContrapartida; // decimal
	public $valorContrapartidaBensServicos; // decimal
	public $valorContrapartidaFinanceira; // decimal
	public $valorGlobal; // decimal
	public $valorPropostaVoluntaria; // parametrosValorPropostaVoluntariaWS
	public $valorRepasse; // decimal
	public $valoresBeneficiarioEmendaParlamentar; // parametrosValoresBeneficiarioEmendaParlamentarWS
}

class parametrosObjetoProgramaWS {
	public $nome; // string
}

class parametrosValorBeneficiarioEspecificoWS {
	public $valor; // decimal
}

class parametrosValorPropostaVoluntariaWS {
	public $valor; // decimal
}

class parametrosValoresBeneficiarioEmendaParlamentarWS {
	public $numeroEmendaParlamentar; // string
	public $valor; // decimal
}

class atualizarProgramasResponse {
	public $return; // string
}

class obterVersaoWebService {
}

class obterVersaoWebServiceResponse {
	public $return; // string
}

class atualizarResponsavelConcedente {
	public $arg0; // parametrosAtualizarResponsavelConcedenteWS
}

class parametrosAtualizarResponsavelConcedenteWS {
	public $anoConvenio; // int
	public $cpfResponsavel; // string
	public $login; // loginWS
	public $sequencialConvenio; // int
}

class atualizarResponsavelConcedenteResponse {
	public $return; // string
}

class unidadeFederativaWS {
	public $id; // long
	public $idHash; // string
	public $sigla; // string
}

class propostaConvenioWS {
	public $ano; // int
	public $id; // long
	public $idHash; // string
	public $sequencial; // int
}
/*
class propostaWS {
	public $ano; // int
	public $atribuicaoRespAnalise; // string
	public $bensServicoWS; // bensServicosPropostaWS
	public $capacidadeTecnica; // string
	public $contaBancariaWS; // contaBancariaWS
	public $cronogramaDesembolsoWS; // cronogramaDesembolsoPropostaWS
	public $cronogramaFisicoWS; // cronogramaFisicoPropostaWS
	public $dataAprovacaoDePropostaWS; // dateTime
	public $dataAprovacaoPlanoTrabalhoWS; // dateTime
	public $dataEntregaProjetoBasicoWS; // dateTime
	public $dataLimiteComplementacaoPbWS; // dateTime
	public $dataLimiteEntregaProjetoBasicoWS; // dateTime
	public $dataPropostaWS; // dateTime
	public $dataUltimoEnvioContratoRepasseWS; // dateTime
	public $dataVersaoWS; // dateTime
	public $despesaAdministrativa; // despesaAdministrativaWS
	public $empenhoPublicacao; // string
	public $executorWS; // participantesPropostaWS
	public $fimExecucao; // dateTime
	public $gestao; // string
	public $gestaoContratoPublicacao; // string
	public $historico; // boolean
	public $id; // long
	public $idHash; // string
	public $inicioExecucao; // dateTime
	public $instituicaoMandatariaWS; // orgaoAdministrativoWS
	public $intervenientesProposta; // intervenientePropostaWS
	public $justificativa; // string
	public $login; // loginWS
	public $mandatarioWS; // participantesPropostaWS
	public $membroParticipanteWS; // membroParticipanteWS
	public $modalidadePropostaWS; // modalidadePropostaWS
	public $motivosNaoAcatamentoContratoRepasse; // motivoNaoAcatamentoContratoRepasseWS
	public $numeroControleExterno; // string
	public $numeroConvenio; // string
	public $objetoConvenio; // string
	public $orgaoAdministrativoWS; // orgaoAdministrativoWS
	public $orgaoConcedenteWS; // orgaoAdministrativoWS
	public $orgaoExecutorWS; // orgaoAdministrativoWS
	public $parecerPlanoTrabalho; // parecerPlanoTrabalhoWS
	public $parecerProposta; // parecerPropostaConvenioWS
	public $permiteAdiarEntregaProjBasico; // boolean
	public $permiteLiberarPrimeiroRepasse; // boolean
	public $permiteProrrogarEntregaProjBasico; // boolean
	public $prazoEntregaProjBasico; // int
	public $proponenteWS; // participantesPropostaWS
	public $propostaParticipeMembroWS; // responsaveisConvenioWS
	public $propostaProgramaWS; // propostaProgramaWS
	public $repasseExercicioFuturoPropostaWS; // repasseExercicioFuturoPropostaWS
	public $sequencial; // int
	public $situacaoEnvioContratoRepasseWS; // situacaoEnvioContratoRepasseWS
	public $situacaoLegado; // int
	public $situacaoProjetoBasicoWS; // situacaoProjetoBasicoWS
	public $situacaoPropostaWS; // situacaoPropostaWS
	public $tipoNaoAcatamentoContratoRepasseWS; // tipoNaoAcatamentoContratoRepasseWS
	public $tipoProjetoBasicoWS; // tipoProjetoBasicoWS
	public $transferenciasRecursos; // transferenciaRecursosWS
	public $valorContraPartida; // double
	public $valorContrapartidaBensServicos; // double
	public $valorContrapartidaFinanceira; // double
	public $valorGlobal; // double
	public $valorRepasse; // double
}*/

class bensServicosPropostaWS {
	public $cep; // string
	public $descricao; // string
	public $despesaCompartilhada; // boolean
	public $endereco; // string
	public $id; // long
	public $idHash; // string
	public $justificativaAnalista; // string
	public $municipioWS; // municipioWS
	public $naturezaAquisicaoWS; // naturezaAquisicaoWS
	public $naturezaDespesaSubItemWS; // naturezaDespesaSubItemWS
	public $observacao; // string
	public $propostaProgramaWS; // propostaProgramaWS
	public $quantidade; // double
	public $statusBensServicosPropostaWS; // statusBensServicosPropostaWS
	public $tipoDespesaWS; // string
	public $unidadeFornecimento; // string
	public $valorTotal; // double
	public $valorUnitario; // double
}

class municipioWS {
	public $codigo; // string
	public $id; // long
	public $idHash; // string
	public $nome; // string
	public $unidadeFederativaWS; // unidadeFederativaWS
}

class naturezaAquisicaoWS {
	public $codigo; // string
	public $descricao; // string
	public $id; // long
}

class naturezaDespesaSubItemWS {
	public $descricaoSubItem; // string
	public $id; // long
	public $idHash; // string
	public $naturezaDespesaWS; // naturezaDespesaWS
	public $observacao; // string
	public $subItem; // string
}

class naturezaDespesaWS {
	public $codigo; // string
	public $descricao; // string
	public $id; // long
	public $idHash; // string
}

class propostaProgramaWS {
	public $id; // long
	public $idHash; // string
	public $objetoWS; // objetoWS
	public $origemRecursoPropProgramaWS; // origemRecursoPropProgramaWS
	public $programaWS; // programaWS
	public $qualificacaoProponenteWS; // qualificacaoProponenteWS
	public $regrasWS; // regrasWS
	public $valorContrapartida; // double
	public $valorContrapartidaBensServicos; // double
	public $valorContrapartidaFinanceira; // double
	public $valorGlobal; // double
}

class objetoWS {
	public $ehObjetoPadronizado; // boolean
	public $id; // long
	public $idHash; // string
	public $justificativa; // string
	public $nome; // string
}

class origemRecursoPropProgramaWS {
	public $cnpjBeneficiarioEspecificoWS; // cnpjBeneficiarioEspecificoWS
	public $cnpjProgramaEmendaWS; // cnpjProgramaEmendaWS
	public $id; // long
	public $idHash; // string
	public $qualificacaoProponenteWS; // string
	public $valorRepasse; // double
}

class cnpjBeneficiarioEspecificoWS {
	public $cnpj; // string
	public $id; // long
	public $idHash; // string
	public $valorRepasseProposta; // double
}

class cnpjProgramaEmendaWS {
	public $cnpj; // string
	public $id; // long
	public $idHash; // string
	public $numeroEmendaParlamentar; // string
	public $valorRepasseProposta; // double
}

class programaWS {
	public $acaoOrcamentaria; // string
	public $aceitaDespesaAdministrativa; // boolean
	public $aceitaPropostaDeProponenteNaoCadastrado; // boolean
	public $chamamentoPublico; // boolean
	public $cnpjBeneficiarioEspecificoWS; // cnpjBeneficiarioEspecificoWS
	public $cnpjProgramaEmendaWS; // cnpjProgramaEmendaWS
	public $codigo; // string
	public $concedente; // orgaoAdministrativoWS
	public $criteriosDeSelecao; // string
	public $dataDisponibilizacao; // dateTime
	public $dataFimBeneficiarioEspecifico; // dateTime
	public $dataFimEmendaParlamentar; // dateTime
	public $dataFimPropostaVoluntaria; // dateTime
	public $dataInicioBeneficiarioEspecifico; // dateTime
	public $dataInicioEmendaParlamentar; // dateTime
	public $dataInicioPropostaVoluntaria; // dateTime
	public $dataPublicacaoImprensa; // dateTime
	public $descricao; // string
	public $estadosHabilitados; // unidadeFederativaWS
	public $executor; // orgaoAdministrativoWS
	public $id; // long
	public $idHash; // string
	public $modalidades; // modalidadeConvenioWS
	public $naturezasJuridicas; // naturezaJuridicaWS
	public $nome; // string
	public $numeroDocumento; // string
	public $objetoWS; // objetoWS
	public $obrigaPlanoTrabalho; // boolean
	public $observacao; // string
	public $publicadoImprensa; // boolean
	public $qualificacaoBeneficiarioEmendaParlamentar; // boolean
	public $qualificacaoBeneficiarioEspecifico; // boolean
	public $qualificacaoPropostaVoluntaria; // boolean
	public $regrasWS; // regrasWS
	public $status; // string
}

class orgaoAdministrativoWS {
	public $codigo; // string
	public $id; // long
	public $idHash; // string
	public $responsavelWS; // responsaveisConvenioWS
}

class responsaveisConvenioWS {
	public $id; // long
	public $idHash; // string
	public $membroWS; // membroParticipanteWS
	public $participeWS; // participantesPropostaWS
}

class membroParticipanteWS {
	public $ativoNoSistema; // boolean
	public $cargoFuncao; // string
	public $cep; // string
	public $cpf; // string
	public $email; // string
	public $endereco; // string
	public $id; // long
	public $idHash; // string
	public $matricula; // string
	public $municipioMembroWS; // municipioWS
	public $municipioWS; // municipioWS
	public $nome; // string
	public $orgaoExpedidor; // string
	public $rg; // string
	public $senha; // string
	public $telefone; // string
}

class participantesPropostaWS {
	public $agencia; // string
	public $alteracaoEstatuaria; // alteracaoEstatuariaWS
	public $aprovado; // boolean
	public $areaAtuacaoEntidadePrivadaWS; // areaAtuacaoEntidadePrivadaWS
	public $bairroDistrito; // string
	public $cep; // string
	public $certidaoAprovada; // boolean
	public $certidaoWS; // certidaoWS
	public $cienteDirigenteNaoRemunerado; // boolean
	public $cnaePrimario; // string
	public $cnaePrimarioWS; // cnaeWS
	public $codigoBanco; // string
	public $codigoErroSiafi; // string
	public $consorcioPublico; // boolean
	public $contaCorrente; // string
	public $cpfUsuarioAprovou; // string
	public $declaracaoAprovado; // boolean
	public $declaracaoNaoDividaWS; // declaracaoNaoDividaWS
	public $email; // string
	public $endereco; // string
	public $entidadesVinculadas; // string
	public $erroSiafi; // string
	public $esferaAdministrativaWS; // esferaAdministrativaWS
	public $estatutoAprovado; // boolean
	public $estatutoWS; // estatutoWS
	public $executor; // boolean
	public $id; // long
	public $idHash; // string
	public $identificacao; // string
	public $inscricaoEstadual; // string
	public $inscricaoMunicipal; // string
	public $interveniente; // boolean
	public $login; // loginWS
	public $mandatario; // boolean
	public $municWS; // municipioWS
	public $naturezaJuridica; // string
	public $nome; // string
	public $nomeFantasia; // string
	public $proponente; // boolean
	public $quadroDirigenteAprovado; // boolean
	public $representanteWS; // membroParticipanteWS
	public $respExercicioWS; // membroParticipanteWS
	public $responsavelWS; // membroParticipanteWS
	public $respostaEnvioSiafi; // string
	public $statusEnvioSiafi; // string
	public $telefone; // string
	public $telexFaxCaixaPostal; // string
	public $tipoIdentificacaoWS; // tipoIdentificadorParticipeWS
	public $transcricaoEstatutoONG; // string
}

class alteracaoEstatuariaWS {
	public $data; // dateTime
	public $id; // long
	public $idHash; // string
	public $texto; // string
}

class areaAtuacaoEntidadePrivadaWS {
	public $codigo; // string
	public $descricao; // string
	public $id; // long
	public $idHash; // string
}

class certidaoWS {
	public $fgtsDataEmissao; // dateTime
	public $fgtsDataValidade; // dateTime
	public $fgtsHora; // string
	public $fgtsIsento; // boolean
	public $fgtsNumero; // string
	public $fgtsTipo; // string
	public $id; // long
	public $idHash; // string
	public $inssDataEmissao; // dateTime
	public $inssDataValidade; // dateTime
	public $inssHora; // string
	public $inssIsento; // boolean
	public $inssNumero; // string
	public $inssTipo; // string
	public $outrasCertidoesWS; // respostaCertidaoWS
	public $receitaEstadualDataEmissao; // dateTime
	public $receitaEstadualDataValidade; // dateTime
	public $receitaEstadualHora; // string
	public $receitaEstadualIsento; // boolean
	public $receitaEstadualNumero; // string
	public $receitaEstadualTipo; // string
	public $receitaMunicipalDataEmissao; // dateTime
	public $receitaMunicipalDataValidade; // dateTime
	public $receitaMunicipalHora; // string
	public $receitaMunicipalIsento; // boolean
	public $receitaMunicipalNumero; // string
	public $receitaMunicipalTipo; // string
	public $srfPgfnDataEmissao; // dateTime
	public $srfPgfnDataValidade; // dateTime
	public $srfPgfnHora; // string
	public $srfPgfnIsento; // boolean
	public $srfPgfnNumero; // string
	public $srfPgfnTipo; // string
}

class respostaCertidaoWS {
	public $dataValidade; // dateTime
	public $id; // long
	public $idHash; // string
	public $nomeCertidao; // string
	public $possuiCertidao; // boolean
}

class cnaeWS {
	public $codigo; // string
	public $descricao; // string
	public $id; // long
	public $idHash; // string
}

class declaracaoNaoDividaWS {
	public $dataAssinatura; // dateTime
	public $dirigenteSignatarioWS; // dirigenteWS
	public $id; // long
	public $idHash; // string
}

class dirigenteWS {
	public $cargo; // string
	public $cpf; // string
	public $id; // long
	public $idHash; // string
	public $nome; // string
	public $orgaoExpedidor; // string
	public $profissao; // string
	public $rg; // string
}

class esferaAdministrativaWS {
	public $codigo; // string
	public $id; // long
}

class estatutoWS {
	public $cartorio; // string
	public $dataRegistro; // dateTime
	public $id; // long
	public $idHash; // string
	public $livroFolha; // string
	public $municipioWS; // municipioWS
	public $numeroDoRegistroMatricula; // string
	public $transcricaoEstatuto; // string
}

class tipoIdentificadorParticipeWS {
	public $codigo; // string
	public $id; // long
}

class modalidadeConvenioWS {
	public $id; // long
	public $idHash; // string
	public $valor; // string
}

class naturezaJuridicaWS {
	public $id; // long
	public $idHash; // string
	public $valor; // string
}

class regrasWS {
	public $aceitaContrapartidaBens; // boolean
	public $descricao; // string
	public $id; // long
	public $idHash; // string
	public $percentualMaximoContrapartidaBens; // double
	public $percentualMinimoContrapartida; // double
}

class qualificacaoProponenteWS {
	public $id; // long
	public $value; // string
}

class statusBensServicosPropostaWS {
	public $codigo; // string
	public $id; // long
}

class contaBancariaWS {
	public $agencia; // string
	public $bancoWS; // bancoWS
	public $conta; // string
	public $dataRetornoUltimoPedidoAberturaWS; // dateTime
	public $dataUltimaModificacaoWS; // dateTime
	public $dataUltimoEnvioPedidoAberturaWS; // dateTime
	public $descricao; // string
	public $digitoVerificadorAgencia; // string
	public $id; // long
	public $idHash; // string
	public $mensagemErro; // string
	public $msgErroProcessamento; // string
	public $situacao; // string
}

class bancoWS {
	public $codigo; // string
	public $id; // long
	public $idHash; // string
	public $nome; // string
}

class cronogramaDesembolsoPropostaWS {
	public $id; // long
	public $idHash; // string
	public $parcelaCronogramaDesembolsoWS; // parcelaCronogramaDesembolsoWS
}

class parcelaCronogramaDesembolsoWS {
	public $ano; // long
	public $id; // long
	public $idHash; // string
	public $mes; // long
	public $numero; // long
	public $parcelaMetaWS; // parcelaMetaWS
	public $tipoParticipanteWS; // tipoParticipanteWS
	public $valor; // double
}

class parcelaMetaWS {
	public $id; // long
	public $idHash; // string
	public $idMetaCronogramaFisico; // long
	public $parcelaEtapaWS; // parcelaEtapaWS
	public $valor; // double
}

class parcelaEtapaWS {
	public $id; // long
	public $idEtapaCronogramaFisico; // long
	public $idHash; // string
	public $valor; // double
}

class tipoParticipanteWS {
	public $codigo; // string
	public $descricao; // string
	public $id; // long
}

class cronogramaFisicoPropostaWS {
	public $id; // long
	public $idHash; // string
	public $metaCronogramaFisicoWS; // metaCronogramaFisicoWS
}

class metaCronogramaFisicoWS {
	public $cep; // string
	public $dataFim; // dateTime
	public $dataInicio; // dateTime
	public $endereco; // string
	public $especificacao; // string
	public $etapaCronogramaFisicoWS; // etapaCronogramaFisicoWS
	public $id; // long
	public $idHash; // string
	public $municipioWS; // municipioWS
	public $propostaProgramaWS; // propostaProgramaWS
	public $quantidade; // double
	public $tipoMeta; // string
	public $unidadeFornecimento; // string
	public $valor; // double
}

class etapaCronogramaFisicoWS {
	public $cep; // string
	public $dataFim; // dateTime
	public $dataInicio; // dateTime
	public $endereco; // string
	public $especificacao; // string
	public $id; // long
	public $idHash; // string
	public $municipioWS; // municipioWS
	public $quantidade; // double
	public $unidadeFornecimento; // string
	public $valor; // double
}

class despesaAdministrativaWS {
	public $cep; // string
	public $compartilhada; // boolean
	public $descricao; // string
	public $endereco; // string
	public $id; // long
	public $idHash; // string
	public $municipio; // municipioWS
	public $naturezaAquisicao; // string
	public $naturezaDespesaSubItem; // naturezaDespesaSubItemWS
	public $numeroConvenio; // string
	public $observacao; // string
	public $programa; // programaWS
	public $quantidade; // double
	public $tipoDespesa; // string
	public $unidadeFornecimento; // string
	public $valorUnitario; // double
}

class intervenientePropostaWS {
	public $orgaoAdministrativo; // orgaoAdministrativoWS
	public $participanteProposta; // participantesPropostaWS
}

class modalidadePropostaWS {
	public $id; // long
	public $value; // string
}

class motivoNaoAcatamentoContratoRepasseWS {
	public $id; // long
	public $idHash; // string
	public $value; // string
}

class parecerPlanoTrabalhoWS {
	public $atribuicaoResponsavel; // string
	public $data; // dateTime
	public $favoravelWS; // string
	public $funcaoResponsavel; // string
	public $id; // long
	public $idHash; // string
	public $parecer; // string
	public $responsavelWS; // membroParticipanteWS
	public $tipoParecerWS; // string
	public $tipoParticipanteWS; // string
}

class parecerPropostaConvenioWS {
	public $atribuicaoResponsavel; // string
	public $dataWS; // dateTime
	public $funcaoResponsavel; // string
	public $id; // long
	public $idHash; // string
	public $parecer; // string
	public $responsavelWS; // membroParticipanteWS
	public $tipoParticipanteWS; // string
}

class repasseExercicioFuturoPropostaWS {
	public $ano; // int
	public $historico; // boolean
	public $id; // long
	public $idHash; // string
	public $valor; // double
}

class situacaoEnvioContratoRepasseWS {
	public $id; // long
	public $value; // string
}

class situacaoProjetoBasicoWS {
	public $id; // long
	public $value; // string
}

class situacaoPropostaWS {
	public $id; // long
	public $value; // string
}

class tipoNaoAcatamentoContratoRepasseWS {
	public $id; // long
	public $value; // string
}

class tipoProjetoBasicoWS {
	public $id; // long
	public $value; // string
}

class transferenciaRecursosWS {
	public $ano; // long
	public $codigoCancelamento; // string
	public $codigoErroSIAFI; // string
	public $codigoGestaoEmitente; // string
	public $codigoUgEmitente; // string
	public $data; // dateTime
	public $dataCancelamento; // dateTime
	public $erroSIAFI; // string
	public $gestaoUgFavorecida; // string
	public $id; // long
	public $idHash; // string
	public $numeroCancelamento; // string
	public $numeroInterno; // long
	public $numeroSIAFI; // string
	public $OBConfluxoExportacao; // obConfluxoExportacaoWS
	public $observacao; // string
	public $observacaoCancelamento; // string
	public $ordemBancariaExportacao; // ordemBancariaExportacaoWS
	public $programacaoFinanceiraExportacao; // programacaoFinanceiraExportacaoWS
	public $valor; // double
}

class obConfluxoExportacaoWS {
	public $anoDocDh; // string
	public $dadosOrcamentariosWS; // dadosOrcamentariosWS
	public $gestaoEmitenteDh; // string
	public $id; // long
	public $idHash; // string
	public $numeroDh; // string
	public $textoJustificativaInadimplencia; // string
	public $tipoDh; // string
	public $ugEmitenteDh; // string
}

class dadosOrcamentariosWS {
	public $categoriaGasto; // boolean
	public $id; // long
	public $idHash; // string
	public $numeroEmpenho; // string
	public $valorGrupo; // double
	public $vinculacao; // string
}

class ordemBancariaExportacaoWS {
	public $codigoAgenciaDestino; // string
	public $codigoAgenciaOrigem; // string
	public $codigoBancoDestino; // string
	public $codigoBancoOrigem; // string
	public $codigoFavorecido; // string
	public $controleStnOriginal; // string
	public $eventosObWS; // eventoOrdemBancariaWS
	public $gestaoDocumentoOrigem; // string
	public $id; // long
	public $idHash; // string
	public $identificadorTransferencia; // string
	public $numeroContaCorrenteDestino; // string
	public $numeroContaCorrenteOrigem; // string
	public $numeroDocumentoOrigem; // string
	public $numeroInternoConcedente; // string
	public $numeroListaOb; // string
	public $numeroProcesso; // string
	public $opcaoInversaoSaldo; // boolean
	public $taxaCambio; // double
	public $ugDocumentoOrigem; // string
}

class programacaoFinanceiraExportacaoWS {
	public $codigoFavorecidoOB; // string
	public $codigoGestaoFavorecidaOB; // string
	public $id; // long
	public $idHash; // string
	public $ugFavorecido; // long
}

class prorrogaOficioWS {
	public $assinado; // boolean
	public $cpfResponsavelAssinatura; // string
	public $dataAssinatura; // dateTime
	public $dataCadastro; // dateTime
	public $dataDisponibilizacao; // dateTime
	public $dataFimVigencia; // dateTime
	public $dataPublicacao; // dateTime
	public $diasProrrogacao; // int
	public $finalizado; // boolean
	public $id; // long
	public $idHash; // string
	public $nomeResponsavelAssinatura; // string
	public $numero; // string
	public $publicado; // boolean
	public $publicar; // boolean
	public $responsavelRegistroWS; // membroParticipanteWS
	public $statusWS; // string
}

class termoAditivoWS {
	public $ano; // int
	public $assinado; // boolean
	public $codigoEnvioSiafi; // long
	public $codigoEnvioSiafiContrapartida; // long
	public $codigoErroSiafi; // string
	public $data; // dateTime
	public $dataAssinatura; // dateTime
	public $dataEnvioXml; // dateTime
	public $dataFimVigencia; // dateTime
	public $dataInicioVigencia; // dateTime
	public $dataPublicacao; // dateTime
	public $descricaoOutros; // string
	public $erroSiafi; // string
	public $finalizado; // boolean
	public $fundamentoLegal; // string
	public $id; // long
	public $idHash; // string
	public $justificativa; // string
	public $numeroNS; // string
	public $numeroNSContrapartida; // string
	public $objetoAlteracao; // string
	public $permiteAlteracoes; // boolean
	public $permitirEdicaoConvenente; // boolean
	public $publicado; // boolean
	public $publicar; // boolean
	public $repassesExercicioFuturosTAWS; // repasseExercicioFuturoTermoAditivoWS
	public $respConcedenteWS; // membroParticipanteWS
	public $respRegistroWS; // membroParticipanteWS
	public $sequencial; // int
	public $situacaoWS; // string
	public $statusWS; // string
	public $textoAmpliacaoObjeto; // string
	public $tiposTAWS; // tipoTaWS
	public $valorContrapartida; // double
	public $valorContrapartidaBensServicos; // double
	public $valorContrapartidaFinanceira; // double
	public $valorGlobal; // double
	public $valorRepasse; // double
}

class repasseExercicioFuturoTermoAditivoWS {
	public $ano; // int
	public $id; // long
	public $idHash; // string
	public $valor; // double
}

class tipoTaWS {
	public $codigoTipo; // string
	public $id; // long
	public $idHash; // string
}

class enviarNotaEmpenhoResponse {
	public $return; // string
}

class atualizarCronogramaFisico {
	public $arg0; // parametrosAtualizarCronogramaFisicoWS
}

class parametrosAtualizarCronogramaFisicoWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $metas; // parametrosMetaCronogramaFisicoWS
	public $sequencialConvenio; // int
}

class parametrosMetaCronogramaFisicoWS {
	public $cep; // string
	public $codigoMunicipio; // string
	public $codigoPrograma; // string
	public $dataFim; // dateTime
	public $dataInicio; // dateTime
	public $endereco; // string
	public $especificacao; // string
	public $etapas; // parametrosEtapaCronogramaFisicoWS
	public $quantidade; // decimal
	public $unidadeFornecimento; // string
	public $valor; // decimal
}

class parametrosEtapaCronogramaFisicoWS {
	public $cep; // string
	public $codigoMunicipio; // string
	public $dataFim; // dateTime
	public $dataInicio; // dateTime
	public $endereco; // string
	public $especificacao; // string
	public $quantidade; // decimal
	public $unidadeFornecimento; // string
	public $valor; // decimal
}

class atualizarCronogramaFisicoResponse {
	public $return; // string
}

class atualizarPareceresProposta {
	public $arg0; // parametrosAtualizarPareceresPropostaWS
}

class parametrosAtualizarPareceresPropostaWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $pareceresProposta; // parametrosParecerPropostaWS
	public $sequencialConvenio; // int
}

class parametrosParecerPropostaWS {
	public $atribuicaoResponsavel; // string
	public $cpfResponsavel; // string
	public $data; // dateTime
	public $funcaoResponsavel; // string
	public $parecer; // string
}

class atualizarPareceresPropostaResponse {
	public $return; // string
}

class atualizarIntervenientes {
	public $arg0; // parametrosAtualizarIntervenientesWS
}

class parametrosAtualizarIntervenientesWS {
	public $anoConvenio; // int
	public $intervenientes; // parametrosIntervenenteWS
	public $login; // loginWS
	public $sequencialConvenio; // int
}

class parametrosIntervenenteWS {
	public $cnpj; // string
}

class atualizarIntervenientesResponse {
	public $return; // string
}

class atualizarRepasses {
	public $arg0; // parametrosAtualizarRepassesWS
}

class parametrosAtualizarRepassesWS {
	public $anoConvenio; // int
	public $login; // loginWS
	public $repasses; // parametrosRepasseWS
	public $sequencialConvenio; // int
}

class parametrosRepasseWS {
	public $ano; // int
	public $valor; // decimal
}

class atualizarRepassesResponse {
	public $return; // string
}

class atualizarDadosConvenio {
	public $arg0; // parametrosAtualizarDadosConvenioWS
}

class parametrosAtualizarDadosConvenioWS {
	public $anoConvenio; // int
	public $capacidadeTecnica; // string
	public $codigoAgenciaBancaria; // string
	public $codigoBanco; // string
	public $digitoVerificadorAgenciaBancaria; // string
	public $fimVigencia; // dateTime
	public $fundamentoLegal; // string
	public $inicioVigencia; // dateTime
	public $justificativa; // string
	public $login; // loginWS
	public $numeroInterno; // string
	public $numeroProcesso; // string
	public $objetoConvenio; // string
	public $sequencialConvenio; // int
}

class atualizarDadosConvenioResponse {
	public $return; // string
}


/**
 * SimecWsFacadeService class
 * 
 *	
 * 
 * @author		{author}
 * @copyright {copyright}
 * @package	 {package}
 */
class SimecWsSiconvConvenio extends SoapClient {
	protected $soapClient;
	private $arrDados;
	private $tipolog;
	private $logid;
	private $db;
	
	private $username;
	private $password;
	
	private static $classmap = array(
									'enviarNotaLancamento' => 'enviarNotaLancamento',
									'notaLancamentoEnvioWS' => 'notaLancamentoEnvioWS',
									'abstractWS' => 'abstractWS',
									'loginWS' => 'loginWS',
									'notaLancamentoWS' => 'notaLancamentoWS',
									'eventoNotaLancamentoWS' => 'eventoNotaLancamentoWS',
									'enviarNotaLancamentoResponse' => 'enviarNotaLancamentoResponse',
									'atualizarCronogramaDesembolso' => 'atualizarCronogramaDesembolso',
									'parametrosAtualizarCronogramaDesembolsoWS' => 'parametrosAtualizarCronogramaDesembolsoWS',
									'parametrosParcelaCronogramaDesembolsoWS' => 'parametrosParcelaCronogramaDesembolsoWS',
									'parametrosParcelaMetaWS' => 'parametrosParcelaMetaWS',
									'parametrosParcelaEtapaWS' => 'parametrosParcelaEtapaWS',
									'atualizarCronogramaDesembolsoResponse' => 'atualizarCronogramaDesembolsoResponse',
									'atualizarPareceresPlanoTrabalho' => 'atualizarPareceresPlanoTrabalho',
									'parametrosAtualizarPareceresPlanoTrabalhoWS' => 'parametrosAtualizarPareceresPlanoTrabalhoWS',
									'parametrosParecerPlanoTrabalhoWS' => 'parametrosParecerPlanoTrabalhoWS',
									'atualizarPareceresPlanoTrabalhoResponse' => 'atualizarPareceresPlanoTrabalhoResponse',
									'enviarOrdemBancaria' => 'enviarOrdemBancaria',
									'ordemBancariaEnvioWS' => 'ordemBancariaEnvioWS',
									'ordemBancariaWS' => 'ordemBancariaWS',
									'eventoOrdemBancariaWS' => 'eventoOrdemBancariaWS',
									'enviarOrdemBancariaResponse' => 'enviarOrdemBancariaResponse',
									'atualizarProgramas' => 'atualizarProgramas',
									'parametrosAtualizarProgramasWS' => 'parametrosAtualizarProgramasWS',
									'parametrosProgramaWS' => 'parametrosProgramaWS',
									'parametrosObjetoProgramaWS' => 'parametrosObjetoProgramaWS',
									'parametrosValorBeneficiarioEspecificoWS' => 'parametrosValorBeneficiarioEspecificoWS',
									'parametrosValorPropostaVoluntariaWS' => 'parametrosValorPropostaVoluntariaWS',
									'parametrosValoresBeneficiarioEmendaParlamentarWS' => 'parametrosValoresBeneficiarioEmendaParlamentarWS',
									'atualizarProgramasResponse' => 'atualizarProgramasResponse',
									'obterVersaoWebService' => 'obterVersaoWebService',
									'obterVersaoWebServiceResponse' => 'obterVersaoWebServiceResponse',
									'atualizarResponsavelConcedente' => 'atualizarResponsavelConcedente',
									'parametrosAtualizarResponsavelConcedenteWS' => 'parametrosAtualizarResponsavelConcedenteWS',
									'atualizarResponsavelConcedenteResponse' => 'atualizarResponsavelConcedenteResponse',
									'enviarNotaEmpenho' => 'enviarNotaEmpenho',
									'convenioWS' => 'convenioWS',
									'notaEmpenhoWS' => 'notaEmpenhoWS',
									'cronogramaEmpenhoWS' => 'cronogramaEmpenhoWS',
									'tipoNotaEmpenhoWS' => 'tipoNotaEmpenhoWS',
									'itemEmpenhoWS' => 'itemEmpenhoWS',
									'tipoEmpenhoWS' => 'tipoEmpenhoWS',
									'unidadeFederativaWS' => 'unidadeFederativaWS',
									'propostaConvenioWS' => 'propostaConvenioWS',
									'propostaWS' => 'propostaWS',
									'bensServicosPropostaWS' => 'bensServicosPropostaWS',
									'municipioWS' => 'municipioWS',
									'naturezaAquisicaoWS' => 'naturezaAquisicaoWS',
									'naturezaDespesaSubItemWS' => 'naturezaDespesaSubItemWS',
									'naturezaDespesaWS' => 'naturezaDespesaWS',
									'propostaProgramaWS' => 'propostaProgramaWS',
									'objetoWS' => 'objetoWS',
									'origemRecursoPropProgramaWS' => 'origemRecursoPropProgramaWS',
									'cnpjBeneficiarioEspecificoWS' => 'cnpjBeneficiarioEspecificoWS',
									'cnpjProgramaEmendaWS' => 'cnpjProgramaEmendaWS',
									'programaWS' => 'programaWS',
									'orgaoAdministrativoWS' => 'orgaoAdministrativoWS',
									'responsaveisConvenioWS' => 'responsaveisConvenioWS',
									'membroParticipanteWS' => 'membroParticipanteWS',
									'participantesPropostaWS' => 'participantesPropostaWS',
									'alteracaoEstatuariaWS' => 'alteracaoEstatuariaWS',
									'areaAtuacaoEntidadePrivadaWS' => 'areaAtuacaoEntidadePrivadaWS',
									'certidaoWS' => 'certidaoWS',
									'respostaCertidaoWS' => 'respostaCertidaoWS',
									'cnaeWS' => 'cnaeWS',
									'declaracaoNaoDividaWS' => 'declaracaoNaoDividaWS',
									'dirigenteWS' => 'dirigenteWS',
									'esferaAdministrativaWS' => 'esferaAdministrativaWS',
									'estatutoWS' => 'estatutoWS',
									'tipoIdentificadorParticipeWS' => 'tipoIdentificadorParticipeWS',
									'modalidadeConvenioWS' => 'modalidadeConvenioWS',
									'naturezaJuridicaWS' => 'naturezaJuridicaWS',
									'regrasWS' => 'regrasWS',
									'qualificacaoProponenteWS' => 'qualificacaoProponenteWS',
									'statusBensServicosPropostaWS' => 'statusBensServicosPropostaWS',
									'contaBancariaWS' => 'contaBancariaWS',
									'bancoWS' => 'bancoWS',
									'cronogramaDesembolsoPropostaWS' => 'cronogramaDesembolsoPropostaWS',
									'parcelaCronogramaDesembolsoWS' => 'parcelaCronogramaDesembolsoWS',
									'parcelaMetaWS' => 'parcelaMetaWS',
									'parcelaEtapaWS' => 'parcelaEtapaWS',
									'tipoParticipanteWS' => 'tipoParticipanteWS',
									'cronogramaFisicoPropostaWS' => 'cronogramaFisicoPropostaWS',
									'metaCronogramaFisicoWS' => 'metaCronogramaFisicoWS',
									'etapaCronogramaFisicoWS' => 'etapaCronogramaFisicoWS',
									'despesaAdministrativaWS' => 'despesaAdministrativaWS',
									'intervenientePropostaWS' => 'intervenientePropostaWS',
									'modalidadePropostaWS' => 'modalidadePropostaWS',
									'motivoNaoAcatamentoContratoRepasseWS' => 'motivoNaoAcatamentoContratoRepasseWS',
									'parecerPlanoTrabalhoWS' => 'parecerPlanoTrabalhoWS',
									'parecerPropostaConvenioWS' => 'parecerPropostaConvenioWS',
									'repasseExercicioFuturoPropostaWS' => 'repasseExercicioFuturoPropostaWS',
									'situacaoEnvioContratoRepasseWS' => 'situacaoEnvioContratoRepasseWS',
									'situacaoProjetoBasicoWS' => 'situacaoProjetoBasicoWS',
									'situacaoPropostaWS' => 'situacaoPropostaWS',
									'tipoNaoAcatamentoContratoRepasseWS' => 'tipoNaoAcatamentoContratoRepasseWS',
									'tipoProjetoBasicoWS' => 'tipoProjetoBasicoWS',
									'transferenciaRecursosWS' => 'transferenciaRecursosWS',
									'obConfluxoExportacaoWS' => 'obConfluxoExportacaoWS',
									'dadosOrcamentariosWS' => 'dadosOrcamentariosWS',
									'ordemBancariaExportacaoWS' => 'ordemBancariaExportacaoWS',
									'programacaoFinanceiraExportacaoWS' => 'programacaoFinanceiraExportacaoWS',
									'prorrogaOficioWS' => 'prorrogaOficioWS',
									'termoAditivoWS' => 'termoAditivoWS',
									'repasseExercicioFuturoTermoAditivoWS' => 'repasseExercicioFuturoTermoAditivoWS',
									'tipoTaWS' => 'tipoTaWS',
									'enviarNotaEmpenhoResponse' => 'enviarNotaEmpenhoResponse',
									'atualizarCronogramaFisico' => 'atualizarCronogramaFisico',
									'parametrosAtualizarCronogramaFisicoWS' => 'parametrosAtualizarCronogramaFisicoWS',
									'parametrosMetaCronogramaFisicoWS' => 'parametrosMetaCronogramaFisicoWS',
									'parametrosEtapaCronogramaFisicoWS' => 'parametrosEtapaCronogramaFisicoWS',
									'atualizarCronogramaFisicoResponse' => 'atualizarCronogramaFisicoResponse',
									'atualizarPareceresProposta' => 'atualizarPareceresProposta',
									'parametrosAtualizarPareceresPropostaWS' => 'parametrosAtualizarPareceresPropostaWS',
									'parametrosParecerPropostaWS' => 'parametrosParecerPropostaWS',
									'atualizarPareceresPropostaResponse' => 'atualizarPareceresPropostaResponse',
									'atualizarIntervenientes' => 'atualizarIntervenientes',
									'parametrosAtualizarIntervenientesWS' => 'parametrosAtualizarIntervenientesWS',
									'parametrosIntervenenteWS' => 'parametrosIntervenenteWS',
									'atualizarIntervenientesResponse' => 'atualizarIntervenientesResponse',
									'atualizarRepasses' => 'atualizarRepasses',
									'parametrosAtualizarRepassesWS' => 'parametrosAtualizarRepassesWS',
									'parametrosRepasseWS' => 'parametrosRepasseWS',
									'atualizarRepassesResponse' => 'atualizarRepassesResponse',
									'atualizarDadosConvenio' => 'atualizarDadosConvenio',
									'parametrosAtualizarDadosConvenioWS' => 'parametrosAtualizarDadosConvenioWS',
									'atualizarDadosConvenioResponse' => 'atualizarDadosConvenioResponse',
								 );

	public function SimecWsSiconvConvenio($wsdl = "https://wshom.convenios.gov.br/siconv-interfaceConvenio-1.0/InterfaceConvenioHandler/InterfaceConvenioHandlerBeanImpl?wsdl", $options = array()) {
		foreach(self::$classmap as $key => $value) {
			if(!isset($options['classmap'][$key])) {
				$options['classmap'][$key] = $value;
			}
		}
		parent::__construct($wsdl, $options);
		
		$this->soapClient = $client;
		$this->arrDados = '';
		$this->tipolog = '';
		$this->logid = '';
		$this->db = '';
	}

	/**
	 *	
	 *
	 * @param enviarNotaLancamento $parameters
	 * @return enviarNotaLancamentoResponse
	 */
	public function enviarNotaLancamento(enviarNotaLancamento $parameters) {
		return $this->__soapCall('enviarNotaLancamento', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarCronogramaDesembolso $parameters
	 * @return atualizarCronogramaDesembolsoResponse
	 */
	public function atualizarCronogramaDesembolso(atualizarCronogramaDesembolso $parameters) {
		return $this->__soapCall('atualizarCronogramaDesembolso', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarPareceresPlanoTrabalho $parameters
	 * @return atualizarPareceresPlanoTrabalhoResponse
	 */
	public function atualizarPareceresPlanoTrabalho(atualizarPareceresPlanoTrabalho $parameters) {
		return $this->__soapCall('atualizarPareceresPlanoTrabalho', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param enviarOrdemBancaria $parameters
	 * @return enviarOrdemBancariaResponse
	 */
	public function enviarOrdemBancaria(enviarOrdemBancaria $parameters) {
		return $this->__soapCall('enviarOrdemBancaria', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarProgramas $parameters
	 * @return atualizarProgramasResponse
	 */
	public function atualizarProgramas(atualizarProgramas $parameters) {
		return $this->__soapCall('atualizarProgramas', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param obterVersaoWebService $parameters
	 * @return obterVersaoWebServiceResponse
	 */
	public function obterVersaoWebService(obterVersaoWebService $parameters) {
		return $this->__soapCall('obterVersaoWebService', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarResponsavelConcedente $parameters
	 * @return atualizarResponsavelConcedenteResponse
	 */
	public function atualizarResponsavelConcedente(atualizarResponsavelConcedente $parameters) {
		return $this->__soapCall('atualizarResponsavelConcedente', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param enviarNotaEmpenho $parameters
	 * @return enviarNotaEmpenhoResponse
	 */
	public function enviarNotaEmpenho(enviarNotaEmpenho $parameters, $arrDados = array()) {
		$this->arrDados = $arrDados;
		$this->tipolog = 'enviarNotaEmpenho';
	
		return $this->__soapCall('enviarNotaEmpenho', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 ), $this->wssecurity_header()
			);
	}

	/**
	 *	
	 *
	 * @param atualizarCronogramaFisico $parameters
	 * @return atualizarCronogramaFisicoResponse
	 */
	public function atualizarCronogramaFisico(atualizarCronogramaFisico $parameters) {
		return $this->__soapCall('atualizarCronogramaFisico', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarPareceresProposta $parameters
	 * @return atualizarPareceresPropostaResponse
	 */
	public function atualizarPareceresProposta(atualizarPareceresProposta $parameters) {
		return $this->__soapCall('atualizarPareceresProposta', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarIntervenientes $parameters
	 * @return atualizarIntervenientesResponse
	 */
	public function atualizarIntervenientes(atualizarIntervenientes $parameters) {
		return $this->__soapCall('atualizarIntervenientes', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarRepasses $parameters
	 * @return atualizarRepassesResponse
	 */
	public function atualizarRepasses(atualizarRepasses $parameters) {
		return $this->__soapCall('atualizarRepasses', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}

	/**
	 *	
	 *
	 * @param atualizarDadosConvenio $parameters
	 * @return atualizarDadosConvenioResponse
	 */
	public function atualizarDadosConvenio(atualizarDadosConvenio $parameters) {
		return $this->__soapCall('atualizarDadosConvenio', array($parameters),			 array(
						'uri' => 'http://interfaceConvenio.cs.siconv.mp.gov.br',
						'soapaction' => ''
					 )
			);
	}
	
	/*Generates de WSSecurity header*/
	private function wssecurity_header() {
	
		/* The timestamp. The computer must be on time or the server you are
		 * connecting may reject the password digest for security.
		*/
		$timestamp = gmdate('Y-m-d\TH:i:s\Z');
		/* A random word. The use of rand() may repeat the word if the server is
		 * very loaded.
		*/
		$nonce = mt_rand();
		/* This is the right way to create the password digest. Using the
		 * password directly may work also, but it's not secure to transmit it
		* without encryption. And anyway, at least with axis+wss4j, the nonce
		* and timestamp are mandatory anyway.
		*/
		$passdigest = base64_encode(
				pack('H*',
						sha1(
								pack('H*', $nonce) . pack('a*',$timestamp).
								pack('a*',$this->password))));
	
		$auth = '
<wsse:Security xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
	<wsse:UsernameToken wsu:Id="UsernameToken-4">
		<wsse:Username>'.$this->username.'</wsse:Username>
		<wsse:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest">'.$passdigest.'</wsse:Password>
		<wsse:Nonce EncodingType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary">'.base64_encode(pack('H*', $nonce)).'</wsse:Nonce>
		<wsu:Created>'.$timestamp.'</wsu:Created>
	</wsse:UsernameToken>
</wsse:Security>';
		
		/* XSD_ANYXML (or 147) is the code to add xml directly into a SoapVar.
		 * Using other codes such as SOAP_ENC, it's really difficult to set the
		* correct namespace for the variables, so the axis server rejects the
		* xml.
		*/
		$authvalues = new SoapVar($auth,XSD_ANYXML);
		$header = new SoapHeader("http://docs.oasis-open.org/wss/2004/01/oasis-".
				"200401-wss-wssecurity-secext-1.0.xsd", "Security", $authvalues,
				true);
	
		return $header;
	}
	
	/* It's necessary to call it if you want to set a different user and
	 * password
	*/
	public function __setUsernameToken($username, $password) {
		$this->username = $username;
		$this->password = $password;
	}
	
	function __doRequest($request, $location, $action, $version, $one_way = 0){
		$this->log($request, $location, $action, $version, 'request');
	
		$response = SoapClient::__doRequest($request, $location,
				$action, $version,
				$one_way);
	
		$this->log($response, $location, $action, $version, 'response');
		 
		return $response;
	}
	
	public function log($request, $location, $action, $version, $tipo){
		global $db;
			
		$request = str_ireplace( "'", '"', $request );
		$request = utf8_decode($request);
		if( $tipo == 'request' ){
			$sql = "INSERT INTO emenda.logerroswsdl(ptrid, exfid, logtipo, logrequest, logresponse, usucpf, logdatasolicitacao)
					VALUES ({$this->arrDados['ptrid']}, ".($this->arrDados['exfid'] ? $this->arrDados['exfid'] : 'null').", '{$this->tipolog}', '$request', 'null', '{$_SESSION['usucpf']}', now()) returning logid";
			$this->logid = $db->pegaUm( $sql );
			//$_SESSION['emenda']['logid'] = $this->logid;
		} else {
			$sql = "UPDATE emenda.logerroswsdl SET
						logtipo = '{$this->tipolog}',
						logresponse = '$request'
					WHERE
						ptrid = {$this->arrDados['ptrid']}
						and logtipo = '{$this->tipolog}'
						and logid = {$this->logid}";
	
			$db->executar($sql);
		}
		$db->commit();
	}
	
			// route all other method calls directly to soapClient
	public function __call($method, $args){
		// you could also add method_exists check here
		return call_user_func_array(array($this->soapClient, $method), $args);
	}
}
?>