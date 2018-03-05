<?php 

class consultarConvenio {
  public $consultaConvenioDTO; // ConsultaConvenioDTO
  public $login; // LoginWSDTO
}

class ConsultaConvenioDTO {
  public $anExercicio; // string
  public $coProgramaFnde; // string
}

class consultarConvenioResponse {
  public $consultarConvenioReturn; // ConsultaConvenioResponseDTO
}

class ConsultaConvenioResponseDTO {
  public $messages; // ArrayOf_xsd_string
  public $numeroConvenio; // string
}

class consultarEmpenho {
  public $consultaEmpenhoDTO; // ConsultaEmpenhoDTO
  public $login; // LoginWSDTO
}

class consultarEmpenhoResponse {
  public $consultarEmpenhoReturn; // ConsultaEmpenhoResponseDTO
}


class EmpenhoDTO {
  public $anConvenio; // string
  public $anExercicioOriginal; // string
  public $coCentroGestaoSolic; // string
  public $coDescricaoEmpenho; // string
  public $coEsferaOrcamentariaSolic; // string
  public $coEspecieEmpenho; // string
  public $coFonteRecursoSolic; // string
  public $coGestaoEmitente; // string
  public $coNaturezaDespesaSolic; // string
  public $coObservacao; // int
  public $coPlanoInternoSolic; // string
  public $coProgramaFnde; // string
  public $coPtresSolic; // string
  public $coTipoEmpenho; // string
  public $coUnidadeGestoraEmitente; // string
  public $coUnidadeOrcamentariaSolic; // string
  public $nuCgcFavorecido; // string
  public $nuConveio; // string
  public $nuEmpenhoOriginal; // string
  public $nuIdSistema; // string
  public $nuProcesso; // string
  public $nuPropostaSiconv; // string
  public $vlEmpenho; // double
}


class solicitarNotaEmpenho {
  public $empenhoDTO; // EmpenhoDTO
  public $login; // LoginWSDTO
}

class solicitarNotaEmpenhoResponse {
  public $solicitarNotaEmpenhoReturn; // SolicitaNotaEmpenhoResponseDTO
}

class solicitarOrdemBancaria {
  public $movimentoPagamentoDTO; // MovimentoPagamentoDTO
  public $login; // LoginWSDTO
}

class solicitarOrdemBancariaResponse {
  public $solicitarOrdemBancariaReturn; // SolicitaOrdemBancariaResponseDTO
}

class consultarOrdemBancaria {
  public $consultaOBDTO; // ConsultaOrdemBancariaDTO
  public $login; // LoginWSDTO
}

class consultarOrdemBancariaResponse {
  public $consultarOrdemBancariaReturn; // ConsultaOrdemBancariaResponseDTO
}

class consultarPropostaPorUF {
	public $arg0; // parametrosConsultarPropostaPorUFWS
}

class parametrosConsultarPropostaPorUFWS {
	public $codigoOrgao; // string
	public $codigoUF; // string
	public $id; // long
	public $login; // loginWS
}

class abstractWS {
}

class loginWS {
	public $id; // long
	public $senha; // string
	public $usuario; // string
}

class consultarPropostaPorUFResponse {
	public $return; // propostasWS
}

class propostasWS {
	public $id; // long
	public $idHash; // string
	public $propostas; // propostaWS
}

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
}

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

class unidadeFederativaWS {
	public $id; // long
	public $idHash; // string
	public $sigla; // string
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
	public $modalidades; // modalidadekWS
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

class programacaoFinanceiraExportacaoWS {
	public $codigoFavorecidoOB; // string
	public $codigoGestaoFavorecidaOB; // string
	public $id; // long
	public $idHash; // string
	public $ugFavorecido; // long
}

class consultarPropostaPorMunicipio {
	public $arg0; // parametrosConsultarPropostaPorMunicipioWS
}

class parametrosConsultarPropostaPorMunicipioWS {
	public $codigoMunicipio; // string
	public $codigoOrgao; // string
	public $id; // long
	public $login; // loginWS
}

class consultarPropostaPorMunicipioResponse {
	public $return; // propostasWS
}

class consultarPropostaPorCNPJ {
	public $arg0; // parametrosConsultarPropostaPorCNPJWS
}

class parametrosConsultarPropostaPorCNPJWS {
	public $cnpj; // string
	public $codigoOrgao; // string
	public $id; // long
	public $login; // loginWS
}

class consultarPropostaPorCNPJResponse {
	public $return; // propostasWS
}

class exportaConvenio {
	public $arg0; // convenioExportacaoWS
}

class convenioExportacaoWS {
	public $ano; // int
	public $codigoOrgaoConcedente; // string
	public $id; // long
	public $idHash; // string
	public $login; // loginWS
	public $sequencial; // int
}

class exportaConvenioResponse {
	public $return; // convenioWS
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

class propostaConvenioWS {
	public $ano; // int
	public $id; // long
	public $idHash; // string
	public $sequencial; // int
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

class consultarPropostaPorPrograma {
	public $arg0; // parametrosConsultarPropostaPorProgramaWS
}

class parametrosConsultarPropostaPorProgramaWS {
	public $codigoOrgao; // string
	public $id; // long
	public $login; // loginWS
	public $numeroPrograma; // string
}

class consultarPropostaPorProgramaResponse {
	public $return; // propostasWS
}

class exportaProponente {
	public $arg0; // proponenteExportacaoWS
}

class proponenteExportacaoWS {
	public $id; // long
	public $identificacao; // string
	public $login; // loginWS
	public $tipoIdentificacao; // string
}

class exportaProponenteResponse {
	public $return; // participantesPropostaWS
}

class enviaProposta {
	public $arg0; // propostaWS
}

class enviaPropostaResponse {
	public $return; // string
}

class consultarPropostaPorAcaoOrcamentaria {
	public $arg0; // parametrosConsultarPropostaPorAcaoOrcamentariaWS
}

class parametrosConsultarPropostaPorAcaoOrcamentariaWS {
	public $codigoOrgao; // string
	public $id; // long
	public $login; // loginWS
	public $numeroAcaoOrcamentaria; // string
}

class consultarPropostaPorAcaoOrcamentariaResponse {
	public $return; // propostasWS
}

class consultarPropostaPorEmendaParlamentar {
	public $arg0; // parametrosConsultarPropostaPorEmendaParlamentarWS
}

class parametrosConsultarPropostaPorEmendaParlamentarWS {
	public $codigoOrgao; // string
	public $id; // long
	public $login; // loginWS
	public $numeroEmendaParlamentar; // string
}

class consultarPropostaPorEmendaParlamentarResponse {
	public $return; // propostasWS
}

class consultarPrograma {
	public $arg0; // parametrosConsultarProgramaWS
}

class parametrosConsultarProgramaWS {
	public $codigoPrograma; // string
	public $id; // long
	public $login; // loginWS
}

class consultarProgramaResponse {
	public $return; // programaWS
}

class enviaProponente {
	public $arg0; // participantesPropostaWS
}

class enviaProponenteResponse {
	public $return; // string
}

class consultaPropostasOrgao {
	public $arg0; // propostaExportacaoWS
}

class propostaExportacaoWS {
	public $ano; // int
	public $codigoOrgaoConcedente; // string
	public $id; // long
	public $idHash; // string
	public $login; // loginWS
	public $sequencial; // int
}

class consultaPropostasOrgaoResponse {
	public $return; // retornoConsultaPropostasOrgaoWS
}

class retornoConsultaPropostasOrgaoWS {
	public $id; // long
	public $idHash; // string
	public $propostaConsultaWS; // propostaConsultaWS
}

class propostaConsultaWS {
	public $codigoPrograma; // string
	public $id; // long
	public $idHash; // string
	public $identificacaoProponente; // string
	public $numeroProposta; // string
	public $tipoIdentificacaoProponente; // string
}

class obterVersaoWebService {
}

class obterVersaoWebServiceResponse {
	public $return; // string
}

class consultaPropostasAlteradasOrgaoPeriodo {
	public $arg0; // propostasAlteradasExportacaoWS
}

class propostasAlteradasExportacaoWS {
	public $codigoOrgaoConcedente; // string
	public $dataInicialPeriodo; // string
	public $id; // long
	public $idHash; // string
	public $login; // loginWS
	public $tipoPeriodo; // string
}

class consultaPropostasAlteradasOrgaoPeriodoResponse {
	public $return; // propostasWS
}

class exportaProposta {
	public $arg0; // propostaExportacaoWS
}

class exportaPropostaResponse {
	public $return; // propostaWS
}


/**
 * SimecWsInterfaceService class
 *
 *
 *
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class SimecWsInterfaceService extends SoapClient {
	protected $soapClient;
	private $arrDados;
	private $tipolog;
	private $logid;
	private $db;
	
	private $username;
	private $password;
	
	private static $classmap = array(
			'consultarPropostaPorUF' => 'consultarPropostaPorUF',
			'parametrosConsultarPropostaPorUFWS' => 'parametrosConsultarPropostaPorUFWS',
			'consultarConvenio' => 'consultarConvenio',
            'consultarConvenioResponse' => 'consultarConvenioResponse',
			'abstractWS' => 'abstractWS',
			'loginWS' => 'loginWS',
			'consultarPropostaPorUFResponse' => 'consultarPropostaPorUFResponse',
			'propostasWS' => 'propostasWS',
			'propostaWS' => 'propostaWS',
			'bensServicosPropostaWS' => 'bensServicosPropostaWS',
			'municipioWS' => 'municipioWS',
			'unidadeFederativaWS' => 'unidadeFederativaWS',
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
			'eventoOrdemBancariaWS' => 'eventoOrdemBancariaWS',
			'programacaoFinanceiraExportacaoWS' => 'programacaoFinanceiraExportacaoWS',
			'consultarPropostaPorMunicipio' => 'consultarPropostaPorMunicipio',
			'parametrosConsultarPropostaPorMunicipioWS' => 'parametrosConsultarPropostaPorMunicipioWS',
			'consultarPropostaPorMunicipioResponse' => 'consultarPropostaPorMunicipioResponse',
			'consultarPropostaPorCNPJ' => 'consultarPropostaPorCNPJ',
			'parametrosConsultarPropostaPorCNPJWS' => 'parametrosConsultarPropostaPorCNPJWS',
			'consultarPropostaPorCNPJResponse' => 'consultarPropostaPorCNPJResponse',
			'exportaConvenio' => 'exportaConvenio',
			'convenioExportacaoWS' => 'convenioExportacaoWS',
			'exportaConvenioResponse' => 'exportaConvenioResponse',
			'notaEmpenhoWS' => 'notaEmpenhoWS',
			'cronogramaEmpenhoWS' => 'cronogramaEmpenhoWS',
			'tipoNotaEmpenhoWS' => 'tipoNotaEmpenhoWS',
			'itemEmpenhoWS' => 'itemEmpenhoWS',
			'tipoEmpenhoWS' => 'tipoEmpenhoWS',
			'propostaConvenioWS' => 'propostaConvenioWS',
			'prorrogaOficioWS' => 'prorrogaOficioWS',
			'termoAditivoWS' => 'termoAditivoWS',
			'repasseExercicioFuturoTermoAditivoWS' => 'repasseExercicioFuturoTermoAditivoWS',
			'tipoTaWS' => 'tipoTaWS',
			'consultarPropostaPorPrograma' => 'consultarPropostaPorPrograma',
			'parametrosConsultarPropostaPorProgramaWS' => 'parametrosConsultarPropostaPorProgramaWS',
			'consultarPropostaPorProgramaResponse' => 'consultarPropostaPorProgramaResponse',
			'exportaProponente' => 'exportaProponente',
			'proponenteExportacaoWS' => 'proponenteExportacaoWS',
			'exportaProponenteResponse' => 'exportaProponenteResponse',
			'enviaProposta' => 'enviaProposta',
			'enviaPropostaResponse' => 'enviaPropostaResponse',
			'consultarPropostaPorAcaoOrcamentaria' => 'consultarPropostaPorAcaoOrcamentaria',
			'parametrosConsultarPropostaPorAcaoOrcamentariaWS' => 'parametrosConsultarPropostaPorAcaoOrcamentariaWS',
			'consultarPropostaPorAcaoOrcamentariaResponse' => 'consultarPropostaPorAcaoOrcamentariaResponse',
			'consultarPropostaPorEmendaParlamentar' => 'consultarPropostaPorEmendaParlamentar',
			'parametrosConsultarPropostaPorEmendaParlamentarWS' => 'parametrosConsultarPropostaPorEmendaParlamentarWS',
			'consultarPropostaPorEmendaParlamentarResponse' => 'consultarPropostaPorEmendaParlamentarResponse',
			'consultarPrograma' => 'consultarPrograma',
			'parametrosConsultarProgramaWS' => 'parametrosConsultarProgramaWS',
			'consultarProgramaResponse' => 'consultarProgramaResponse',
			'enviaProponente' => 'enviaProponente',
			'enviaProponenteResponse' => 'enviaProponenteResponse',
			'consultaPropostasOrgao' => 'consultaPropostasOrgao',
			'propostaExportacaoWS' => 'propostaExportacaoWS',
			'consultaPropostasOrgaoResponse' => 'consultaPropostasOrgaoResponse',
			'retornoConsultaPropostasOrgaoWS' => 'retornoConsultaPropostasOrgaoWS',
			'propostaConsultaWS' => 'propostaConsultaWS',
			'obterVersaoWebService' => 'obterVersaoWebService',
			'obterVersaoWebServiceResponse' => 'obterVersaoWebServiceResponse',
			'consultaPropostasAlteradasOrgaoPeriodo' => 'consultaPropostasAlteradasOrgaoPeriodo',
			'propostasAlteradasExportacaoWS' => 'propostasAlteradasExportacaoWS',
			'consultaPropostasAlteradasOrgaoPeriodoResponse' => 'consultaPropostasAlteradasOrgaoPeriodoResponse',
			'exportaProposta' => 'exportaProposta',
			'exportaPropostaResponse' => 'exportaPropostaResponse',
	);

	public function SimecWsInterfaceService($wsdl = "https://wshom.convenios.gov.br/siconv-interfaceSiconv-1.0/InterfaceSiconvHandler/InterfaceSiconvHandlerBeanImpl?wsdl", $options = array()) {
            foreach(self::$classmap as $key => $value) {
                if(!isset($options['classmap'][$key])) {
                    $options['classmap'][$key] = $value;
                }
            }
            $this->soapClient = $client;
            $this->arrDados = '';
            $this->tipolog = '';
            $this->logid = '';
            $this->db = '';

            parent::__construct($wsdl, Array(
                'exceptions' => TRUE,
                'trace' => TRUE,
                'stream_context' => stream_context_create(array(
                    "ssl" => array(
                        "verify_peer" => FALSE,
                        "allow_self_signed" => TRUE
                )))
            ));
	}

	/**
	 *
	 *
	 * @param consultarPropostaPorUF $parameters
	 * @return consultarPropostaPorUFResponse
	 */
	public function consultarPropostaPorUF(consultarPropostaPorUF $parameters) {


//	    ver(htmlentities($this->wssecurity_header()->data->enc_value), d);

		return $this->__soapCall(
		    'consultarPropostaPorUF',
            array($parameters),
            array(
//				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'uri' => 'https://ws.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl',
				'soapaction' => ''
		    ),
            $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param consultarPropostaPorCNPJ $parameters
	 * @return consultarPropostaPorCNPJResponse
	 */
	public function consultarPropostaPorCNPJ(consultarPropostaPorCNPJ $parameters) {
		return $this->__soapCall('consultarPropostaPorCNPJ', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param consultarPropostaPorMunicipio $parameters
	 * @return consultarPropostaPorMunicipioResponse
	 */
	public function consultarPropostaPorMunicipio(consultarPropostaPorMunicipio $parameters) {
		return $this->__soapCall('consultarPropostaPorMunicipio', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param exportaConvenio $parameters
	 * @return exportaConvenioResponse
	 */
	public function exportaConvenio(exportaConvenio $parameters, $arrDados = array()) {
		$this->arrDados = $arrDados;
		$this->tipolog = 'exportaConvenio';
	
		return $this->__soapCall(
		    'exportaConvenio',
            array($parameters),
            array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		    ),
            $this->wssecurity_header()
		);
	}
	
/**
   *  
   *
   * @param consultarConvenio $parameters
   * @return consultarConvenioResponse
   */
  public function consultarConvenio(consultarConvenio $parameters, $arrDados = array()) {
  	$this->arrDados = $arrDados;
	$this->tipolog = 'consultarConvenio';
	
	ver(111, d);
		
    return $this->__soapCall('consultarConvenio', array($parameters),       array(
            'uri' => 'http://simec.intraSiconvWS.sistema.fnde.gov.br',
            'soapaction' => ''
           ), $this->wssecurity_header()
      );
  }

	/**
	 *
	 *
	 * @param consultarPropostaPorPrograma $parameters
	 * @return consultarPropostaPorProgramaResponse
	 */
	public function consultarPropostaPorPrograma(consultarPropostaPorPrograma $parameters) {
		return $this->__soapCall('consultarPropostaPorPrograma', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param exportaProponente $parameters
	 * @return exportaProponenteResponse
	 */
	public function exportaProponente(exportaProponente $parameters, $arrDados = array()) {
		$this->arrDados = $arrDados;
		$this->tipolog = 'enviaProposta';
		
		return $this->__soapCall('exportaProponente', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		), $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param enviaProposta $parameters
	 * @return enviaPropostaResponse
	 */
	/* public function __soapCall($function_name, $arguments, $options=null,
			$input_headers=null, $output_headers=null) {
	
		$result = parent::__soapCall($function_name, $arguments, $options,
				$this->wssecurity_header());
	
		return $result;
	} */

	public function enviaPropostaWS(enviaProposta $parameters, $arrDados = array()) {
		$this->arrDados = $arrDados;
		$this->tipolog = 'enviaProposta';
		
		return $this->__soapCall('enviaProposta', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		), $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param consultarPropostaPorAcaoOrcamentaria $parameters
	 * @return consultarPropostaPorAcaoOrcamentariaResponse
	 */
	public function consultarPropostaPorAcaoOrcamentaria(consultarPropostaPorAcaoOrcamentaria $parameters) {
		return $this->__soapCall('consultarPropostaPorAcaoOrcamentaria', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
        ), $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param consultarPrograma $parameters
	 * @return consultarProgramaResponse
	 */
	public function consultarPrograma(consultarPrograma $parameters) {
		return $this->__soapCall('consultarPrograma', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
        ), $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param consultarPropostaPorEmendaParlamentar $parameters
	 * @return consultarPropostaPorEmendaParlamentarResponse
	 */
	public function consultarPropostaPorEmendaParlamentar(consultarPropostaPorEmendaParlamentar $parameters) {
		return $this->__soapCall('consultarPropostaPorEmendaParlamentar', array($parameters),       array(
				'uri' => 'https://ws.convenios.gov.br/siconv-siconv-interfaceSiconv-1.0/InterfaceSiconvHandlerBeanImpl?wsdl',
				'soapaction' => ''
		), $this->wssecurity_header()
		);
	}

	/**
	 *
	 *
	 * @param enviaProponente $parameters
	 * @return enviaProponenteResponse
	 */
	public function enviaProponente(enviaProponente $parameters) {
		return $this->__soapCall('enviaProponente', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param consultaPropostasOrgao $parameters
	 * @return consultaPropostasOrgaoResponse
	 */
	public function consultaPropostasOrgao(consultaPropostasOrgao $parameters) {
		return $this->__soapCall('consultaPropostasOrgao', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
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
		return $this->__soapCall('obterVersaoWebService', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param consultaPropostasAlteradasOrgaoPeriodo $parameters
	 * @return consultaPropostasAlteradasOrgaoPeriodoResponse
	 */
	public function consultaPropostasAlteradasOrgaoPeriodo(consultaPropostasAlteradasOrgaoPeriodo $parameters) {
		return $this->__soapCall('consultaPropostasAlteradasOrgaoPeriodo', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
				'soapaction' => ''
		)
		);
	}

	/**
	 *
	 *
	 * @param exportaProposta $parameters
	 * @return exportaPropostaResponse
	 */
	public function exportaProposta(exportaProposta $parameters) {
		return $this->__soapCall('exportaProposta', array($parameters),       array(
				'uri' => 'http://interfaceSiconv.cs.siconv.mp.gov.br/',
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
		
		/*
		 * 
<wsse:Security SOAP-ENV:mustUnderstand="1" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">
<wsse:UsernameToken>
    <wsse:Username>'.$this->username.'</wsse:Username>
    <wsse:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-'.
	    'wss-username-token-profile-1.0#PasswordDigest">'.$passdigest.'</wsse:Password>
    <wsse:Nonce>'.base64_encode(pack('H*', $nonce)).'</wsse:Nonce>
    <wsu:Created xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-'.
	    '200401-wss-wssecurity-utility-1.0.xsd">'.$timestamp.'</wsu:Created>
   </wsse:UsernameToken>
</wsse:Security>
		 * */
	
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
//		$this->log($request, $location, $action, $version, 'request');
	
		$response = SoapClient::__doRequest($request, $location,
				$action, $version,
				$one_way);
	
//		$this->log($response, $location, $action, $version, 'response');
		 
		return $response;
	}
	
	public function log($request, $location, $action, $version, $tipo){
	
		if($this->db){
			$this->db = $this->db;
		} else {
			$this->db = new cls_banco();
		}
			
		$request = str_ireplace( "'", '"', $request );
		$request = utf8_decode($request);
		if( $tipo == 'request' ){
			$sql = "INSERT INTO emenda.logerroswsdl(ptrid, exfid, logtipo, logrequest, logresponse, usucpf, logdatasolicitacao)
					VALUES ({$this->arrDados['ptrid']}, ".($this->arrDados['exfid'] ? $this->arrDados['exfid'] : 'null').", '{$this->tipolog}', '$request', 'null', '{$_SESSION['usucpf']}', now()) returning logid";
			$this->logid = $this->db->pegaUm( $sql );
			$_SESSION['emenda']['logid'] = $this->logid;
		} else {
			$sql = "UPDATE emenda.logerroswsdl SET
						logtipo = '{$this->tipolog}',
						logresponse = '$request'
					WHERE
						ptrid = {$this->arrDados['ptrid']}
						and logtipo = '{$this->tipolog}'
						and logid = {$this->logid}";
	
			$this->db->executar($sql, false);
		}
		$this->db->commit();
	}
	
			// route all other method calls directly to soapClient
	public function __call($method, $args){
		// you could also add method_exists check here
		return call_user_func_array(array($this->soapClient, $method), $args);
	}
}
?>