<?php
class enviarProposta {
  public $propostaWSDTO; // PropostaWSDTO
  public $login; // LoginWSDTO
}

class enviarPropostaResponse {
  public $enviarPropostaReturn; // PropostaResponseDTO
}

class consultarConvenio {
  public $consultaConvenioDTO; // ConsultaConvenioDTO
  public $login; // LoginWSDTO
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

class solicitarNotaEmpenho {
  public $empenhoDTO; // EmpenhoDTO
  public $login; // LoginWSDTO
}

class solicitarNotaEmpenhoResponse {
  public $solicitarNotaEmpenhoReturn; // long
}

class UnidadeFederativaWSDTO {
  public $id; // long
  public $sigla; // string
}

class MunicipioWSDTO {
  public $codigo; // string
  public $id; // long
  public $nome; // string
  public $unidadeFederativaWS; // UnidadeFederativaWSDTO
}

class NaturezaAquisicaoWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class NaturezaDespesaWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class NaturezaDespesaSubItemWSDTO {
  public $descricaoSubItem; // string
  public $id; // long
  public $naturezaDespesaWS; // NaturezaDespesaWSDTO
  public $observacao; // string
  public $subItem; // string
}

class ObjetoWSDTO {
  public $ehObjetoPadronizado; // boolean
  public $id; // long
  public $justificativa; // string
  public $nome; // string
}

class CnpjBeneficiarioEspecificoWSDTO {
  public $cnpj; // string
  public $id; // long
  public $valorRepasseProposta; // double
}

class CnpjProgramaEmendaWSDTO {
  public $cnpj; // string
  public $id; // long
  public $numeroEmendaParlamentar; // string
  public $valorRepasseProposta; // double
}

class OrigemRecursoPropProgramaWSDTO {
  public $cnpjBeneficiarioEspecificoWS; // CnpjBeneficiarioEspecificoWSDTO
  public $cnpjProgramaEmendaWS; // CnpjProgramaEmendaWSDTO
  public $id; // long
  public $qualificacaoProponenteWS; // string
  public $valorRepasse; // double
}

class MembroParticipanteWSDTO {
  public $ativoNoSistema; // boolean
  public $cargoFuncao; // string
  public $cep; // string
  public $cpf; // string
  public $email; // string
  public $endereco; // string
  public $id; // long
  public $matricula; // string
  public $municipioMembroWS; // MunicipioWSDTO
  public $municipioWS; // MunicipioWSDTO
  public $nome; // string
  public $orgaoExpedidor; // string
  public $rg; // string
  public $senha; // string
  public $telefone; // string
}

class AlteracaoEstatuariaWSDTO {
  public $data; // dateTime
  public $id; // long
  public $texto; // string
}

class AreaAtuacaoEntidadePrivadaWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class RespostaCertidaoWSDTO {
  public $dataValidade; // dateTime
  public $id; // long
  public $nomeCertidao; // string
  public $possuiCertidao; // boolean
}

class CertidaoWSDTO {
  public $fgtsDataEmissao; // dateTime
  public $fgtsDataValidade; // dateTime
  public $fgtsHora; // string
  public $fgtsIsento; // boolean
  public $fgtsNumero; // string
  public $fgtsTipo; // string
  public $id; // long
  public $inssDataEmissao; // dateTime
  public $inssDataValidade; // dateTime
  public $inssHora; // string
  public $inssIsento; // boolean
  public $inssNumero; // string
  public $inssTipo; // string
  public $outrasCertidoesWS; // ArrayOf_tns1_RespostaCertidaoWSDTO
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

class CnaeWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class DirigenteWSDTO {
  public $cargo; // string
  public $cpf; // string
  public $id; // long
  public $nome; // string
  public $orgaoExpedidor; // string
  public $profissao; // string
  public $rg; // string
}

class DeclaracaoNaoDividaWSDTO {
  public $dataAssinatura; // dateTime
  public $dirigenteSignatarioWS; // DirigenteWSDTO
  public $id; // long
}

class EsferaAdministrativaWSDTO {
  public $codigo; // string
  public $id; // long
}

class EstatutoWSDTO {
  public $cartorio; // string
  public $dataRegistro; // dateTime
  public $id; // long
  public $livroFolha; // string
  public $municipioWS; // MunicipioWSDTO
  public $numeroDoRegistroMatricula; // string
  public $transcricaoEstatuto; // string
}

class LoginWSDTO {
  public $id; // long
  public $senha; // string
  public $usuario; // string
}

class TipoIdentificadorParticipeWSDTO {
  public $codigo; // string
  public $id; // long
}

class ParticipantesPropostaWSDTO {
  public $agencia; // string
  public $alteracaoEstatuaria; // ArrayOf_tns1_AlteracaoEstatuariaWSDTO
  public $aprovado; // boolean
  public $areaAtuacaoEntidadePrivadaWS; // ArrayOf_tns1_AreaAtuacaoEntidadePrivadaWSDTO
  public $bairroDistrito; // string
  public $cep; // string
  public $certidaoAprovada; // boolean
  public $certidaoWS; // CertidaoWSDTO
  public $cienteDirigenteNaoRemunerado; // boolean
  public $cnaePrimario; // string
  public $cnaePrimarioWS; // CnaeWSDTO
  public $codigoBanco; // string
  public $codigoErroSiafi; // string
  public $consorcioPublico; // boolean
  public $contaCorrente; // string
  public $cpfUsuarioAprovou; // string
  public $declaracaoAprovado; // boolean
  public $declaracaoNaoDividaWS; // DeclaracaoNaoDividaWSDTO
  public $email; // string
  public $endereco; // string
  public $entidadesVinculadas; // string
  public $erroSiafi; // string
  public $esferaAdministrativaWS; // EsferaAdministrativaWSDTO
  public $estatutoAprovado; // boolean
  public $estatutoWS; // EstatutoWSDTO
  public $executor; // boolean
  public $id; // long
  public $identificacao; // string
  public $inscricaoEstadual; // string
  public $inscricaoMunicipal; // string
  public $interveniente; // boolean
  public $login; // LoginWSDTO
  public $mandatario; // boolean
  public $municWS; // MunicipioWSDTO
  public $naturezaJuridica; // string
  public $nome; // string
  public $nomeFantasia; // string
  public $proponente; // boolean
  public $quadroDirigenteAprovado; // boolean
  public $representanteWS; // MembroParticipanteWSDTO
  public $respExercicioWS; // MembroParticipanteWSDTO
  public $responsavelWS; // MembroParticipanteWSDTO
  public $respostaEnvioSiafi; // string
  public $statusEnvioSiafi; // string
  public $telefone; // string
  public $telexFaxCaixaPostal; // string
  public $tipoIdentificacaoWS; // TipoIdentificadorParticipeWSDTO
  public $transcricaoEstatutoONG; // string
}

class ResponsaveisConvenioWSDTO {
  public $id; // long
  public $membroWS; // MembroParticipanteWSDTO
  public $participeWS; // ParticipantesPropostaWSDTO
}

class OrgaoAdministrativoWSDTO {
  public $codigo; // string
  public $id; // long
  public $responsavelWS; // ResponsaveisConvenioWSDTO
}

class ModalidadeConvenioWSDTO {
  public $id; // long
  public $valor; // string
}

class NaturezaJuridicaWSDTO {
  public $id; // long
  public $valor; // string
}

class RegrasWSDTO {
  public $aceitaContrapartidaBens; // boolean
  public $descricao; // string
  public $id; // long
  public $percentualMaximoContrapartidaBens; // double
  public $percentualMinimoContrapartida; // double
}

class ProgramaWSDTO {
  public $acaoOrcamentaria; // string
  public $aceitaDespesaAdministrativa; // boolean
  public $aceitaPropostaDeProponenteNaoCadastrado; // boolean
  public $chamamentoPublico; // boolean
  public $cnpjBeneficiarioEspecificoWS; // ArrayOf_tns1_CnpjBeneficiarioEspecificoWSDTO
  public $cnpjProgramaEmendaWS; // ArrayOf_tns1_CnpjProgramaEmendaWSDTO
  public $codigo; // string
  public $concedente; // OrgaoAdministrativoWSDTO
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
  public $estadosHabilitados; // ArrayOf_tns1_UnidadeFederativaWSDTO
  public $executor; // OrgaoAdministrativoWSDTO
  public $id; // long
  public $modalidades; // ArrayOf_tns1_ModalidadeConvenioWSDTO
  public $naturezasJuridicas; // ArrayOf_tns1_NaturezaJuridicaWSDTO
  public $nome; // string
  public $numeroDocumento; // string
  public $objetoWS; // ArrayOf_tns1_ObjetoWSDTO
  public $obrigaPlanoTrabalho; // boolean
  public $observacao; // string
  public $publicadoImprensa; // boolean
  public $qualificacaoBeneficiarioEmendaParlamentar; // boolean
  public $qualificacaoBeneficiarioEspecifico; // boolean
  public $qualificacaoPropostaVoluntaria; // boolean
  public $regrasWS; // ArrayOf_tns1_RegrasWSDTO
  public $status; // string
}

class QualificacaoProponenteWSDTO {
  public $id; // long
  public $value; // string
}

class PropostaProgramaWSDTO {
  public $id; // long
  public $objetoWS; // ArrayOf_tns1_ObjetoWSDTO
  public $origemRecursoPropProgramaWS; // ArrayOf_tns1_OrigemRecursoPropProgramaWSDTO
  public $programaWS; // ProgramaWSDTO
  public $qualificacaoProponenteWS; // QualificacaoProponenteWSDTO
  public $regrasWS; // RegrasWSDTO
  public $valorContrapartida; // double
  public $valorContrapartidaBensServicos; // double
  public $valorContrapartidaFinanceira; // double
  public $valorGlobal; // double
}

class StatusBensServicosPropostaWSDTO {
  public $codigo; // string
  public $id; // long
}

class BensServicosPropostaWSDTO {
  public $cep; // string
  public $descricao; // string
  public $despesaCompartilhada; // boolean
  public $endereco; // string
  public $id; // long
  public $justificativaAnalista; // string
  public $municipioWS; // MunicipioWSDTO
  public $naturezaAquisicaoWS; // NaturezaAquisicaoWSDTO
  public $naturezaDespesaSubItemWS; // NaturezaDespesaSubItemWSDTO
  public $observacao; // string
  public $propostaProgramaWS; // PropostaProgramaWSDTO
  public $quantidade; // double
  public $statusBensServicosPropostaWS; // StatusBensServicosPropostaWSDTO
  public $tipoDespesaWS; // string
  public $unidadeFornecimento; // string
  public $valorUnitario; // double
}

class BancoWSDTO {
  public $codigo; // string
  public $id; // long
  public $nome; // string
}

class ContaBancariaWSDTO {
  public $agencia; // string
  public $bancoWS; // BancoWSDTO
  public $conta; // string
  public $dataRetornoUltimoPedidoAberturaWS; // dateTime
  public $dataUltimaModificacaoWS; // dateTime
  public $dataUltimoEnvioPedidoAberturaWS; // dateTime
  public $descricao; // string
  public $digitoVerificadorAgencia; // string
  public $id; // long
  public $mensagemErro; // string
  public $msgErroProcessamento; // string
  public $situacao; // string
}

class ParcelaEtapaWSDTO {
  public $id; // long
  public $idEtapaCronogramaFisico; // long
  public $valor; // double
}

class ParcelaMetaWSDTO {
  public $id; // long
  public $idMetaCronogramaFisico; // long
  public $parcelaEtapaWS; // ArrayOf_tns1_ParcelaEtapaWSDTO
  public $valor; // double
}

class TipoParticipanteWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class DadosOrcamentariosWSDTO {
  public $categoriaGasto; // boolean
  public $id; // long
  public $numeroEmpenho; // string
  public $valorGrupo; // double
  public $vinculacao; // string
}

class OBConfluxoExportacaoWSDTO {
  public $anoDocDh; // string
  public $dadosOrcamentariosWS; // ArrayOf_tns1_DadosOrcamentariosWSDTO
  public $gestaoEmitenteDh; // string
  public $id; // long
  public $numeroDh; // string
  public $textoJustificativaInadimplencia; // string
  public $tipoDh; // string
  public $ugEmitenteDh; // string
}

class EventoOrdemBancariaWSDTO {
  public $classificacao1; // string
  public $classificacao2; // string
  public $codEvento; // string
  public $id; // long
  public $inscricao1; // string
  public $inscricao2; // string
  public $valorLancamento; // double
}

class OrdemBancariaExportacaoWSDTO {
  public $codigoAgenciaDestino; // string
  public $codigoAgenciaOrigem; // string
  public $codigoBancoDestino; // string
  public $codigoBancoOrigem; // string
  public $codigoFavorecido; // string
  public $controleStnOriginal; // string
  public $eventosObWS; // ArrayOf_tns1_EventoOrdemBancariaWSDTO
  public $gestaoDocumentoOrigem; // string
  public $id; // long
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

class ProgramacaoFinanceiraExportacaoWSDTO {
  public $codigoFavorecidoOB; // string
  public $codigoGestaoFavorecidaOB; // string
  public $id; // long
  public $ugFavorecido; // long
}

class TransferenciaRecursosWSDTO {
  public $OBConfluxoExportacao; // OBConfluxoExportacaoWSDTO
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
  public $numeroCancelamento; // string
  public $numeroInterno; // long
  public $numeroSIAFI; // string
  public $observacao; // string
  public $observacaoCancelamento; // string
  public $ordemBancariaExportacao; // OrdemBancariaExportacaoWSDTO
  public $programacaoFinanceiraExportacao; // ProgramacaoFinanceiraExportacaoWSDTO
  public $valor; // double
}

class ParcelaCronogramaDesembolsoWSDTO {
  public $ano; // long
  public $id; // long
  public $mes; // long
  public $numero; // long
  public $parcelaMetaWS; // ArrayOf_tns1_ParcelaMetaWSDTO
  public $tipoParticipanteWS; // TipoParticipanteWSDTO
  public $transferenciasRecursosWS; // ArrayOf_tns1_TransferenciaRecursosWSDTO
  public $valor; // double
}

class CronogramaDesembolsoPropostaWSDTO {
  public $id; // long
  public $parcelaCronogramaDesembolsoWS; // ArrayOf_tns1_ParcelaCronogramaDesembolsoWSDTO
}

class EtapaCronogramaFisicoWSDTO {
  public $cep; // string
  public $dataFim; // dateTime
  public $dataInicio; // dateTime
  public $endereco; // string
  public $especificacao; // string
  public $id; // long
  public $municipioWS; // MunicipioWSDTO
  public $quantidade; // double
  public $unidadeFornecimento; // string
  public $valor; // double
}

class MetaCronogramaFisicoWSDTO {
  public $cep; // string
  public $dataFim; // dateTime
  public $dataInicio; // dateTime
  public $endereco; // string
  public $especificacao; // string
  public $etapaCronogramaFisicoWS; // ArrayOf_tns1_EtapaCronogramaFisicoWSDTO
  public $id; // long
  public $municipioWS; // MunicipioWSDTO
  public $propostaProgramaWS; // PropostaProgramaWSDTO
  public $quantidade; // double
  public $unidadeFornecimento; // string
  public $valor; // double
}

class CronogramaFisicoPropostaWSDTO {
  public $id; // long
  public $metaCronogramaFisicoWS; // ArrayOf_tns1_MetaCronogramaFisicoWSDTO
}

class TipoDespesaAdministrativaWSDTO {
  public $codigo; // string
  public $descricao; // string
  public $id; // long
}

class DespesaAdministrativaWSDTO {
  public $cep; // string
  public $compartilhada; // boolean
  public $descricao; // string
  public $endereco; // string
  public $id; // long
  public $municipio; // MunicipioWSDTO
  public $naturezaAquisicao; // NaturezaAquisicaoWSDTO
  public $naturezaDespesaSubItem; // NaturezaDespesaSubItemWSDTO
  public $numeroConvenio; // string
  public $observacao; // string
  public $programa; // ProgramaWSDTO
  public $quantidade; // double
  public $tipoDespesa; // TipoDespesaAdministrativaWSDTO
  public $unidadeFornecimento; // string
  public $valorUnitario; // double
}

class ModalidadePropostaWSDTO {
  public $id; // long
  public $value; // string
}

class MotivoNaoAcatamentoContratoRepasseWSDTO {
  public $id; // long
  public $value; // string
}

class ParecerPlanoTrabalhoWSDTO {
  public $atribuicaoResponsavel; // string
  public $data; // dateTime
  public $favoravelWS; // string
  public $funcaoResponsavel; // string
  public $id; // long
  public $parecer; // string
  public $responsavelWS; // MembroParticipanteWSDTO
  public $tipoParecerWS; // string
  public $tipoParticipanteWS; // string
}

class ParecerPropostaConvenioWSDTO {
  public $atribuicaoResponsavel; // string
  public $dataWS; // dateTime
  public $funcaoResponsavel; // string
  public $id; // long
  public $parecer; // string
  public $responsavelWS; // MembroParticipanteWSDTO
  public $tipoParticipanteWS; // string
}

class RepasseExercicioFuturoPropostaWSDTO {
  public $ano; // int
  public $historico; // boolean
  public $id; // long
  public $valor; // double
}

class SituacaoEnvioContratoRepasseWSDTO {
  public $id; // long
  public $value; // string
}

class SituacaoProjetoBasicoWSDTO {
  public $id; // long
  public $value; // string
}

class SituacaoPropostaWSDTO {
  public $id; // long
  public $value; // string
}

class TipoNaoAcatamentoContratoRepasseWSDTO {
  public $id; // long
  public $value; // string
}

class TipoProjetoBasicoWSDTO {
  public $id; // long
  public $value; // string
}

class PropostaWSDTO {
  public $ano; // int
  public $atribuicaoRespAnalise; // string
  public $bensServicoWS; // ArrayOf_tns1_BensServicosPropostaWSDTO
  public $capacidadeTecnica; // string
  public $contaBancariaWS; // ContaBancariaWSDTO
  public $cronogramaDesembolsoWS; // CronogramaDesembolsoPropostaWSDTO
  public $cronogramaFisicoWS; // CronogramaFisicoPropostaWSDTO
  public $dataAprovacaoDePropostaWS; // dateTime
  public $dataAprovacaoPlanoTrabalhoWS; // dateTime
  public $dataEntregaProjetoBasicoWS; // dateTime
  public $dataLimiteComplementacaoPbWS; // dateTime
  public $dataLimiteEntregaProjetoBasicoWS; // dateTime
  public $dataPropostaWS; // dateTime
  public $dataUltimoEnvioContratoRepasseWS; // dateTime
  public $dataVersaoWS; // dateTime
  public $despesaAdministrativa; // ArrayOf_tns1_DespesaAdministrativaWSDTO
  public $empenhoPublicacao; // string
  public $executorWS; // ArrayOf_tns1_ParticipantesPropostaWSDTO
  public $fimExecucao; // dateTime
  public $gestao; // string
  public $gestaoContratoPublicacao; // string
  public $historico; // boolean
  public $id; // long
  public $inicioExecucao; // dateTime
  public $instituicaoMandatariaWS; // OrgaoAdministrativoWSDTO
  public $intervenenteWS; // ArrayOf_tns1_ParticipantesPropostaWSDTO
  public $justificativa; // string
  public $login; // LoginWSDTO
  public $mandatarioWS; // ParticipantesPropostaWSDTO
  public $membroParticipanteWS; // MembroParticipanteWSDTO
  public $modalidadePropostaWS; // ModalidadePropostaWSDTO
  public $motivosNaoAcatamentoContratoRepasse; // ArrayOf_tns1_MotivoNaoAcatamentoContratoRepasseWSDTO
  public $objetoConvenio; // string
  public $orgaoAdministrativoWS; // OrgaoAdministrativoWSDTO
  public $orgaoConcedenteWS; // OrgaoAdministrativoWSDTO
  public $orgaoExecutorWS; // OrgaoAdministrativoWSDTO
  public $parecerPlanoTrabalho; // ArrayOf_tns1_ParecerPlanoTrabalhoWSDTO
  public $parecerProposta; // ArrayOf_tns1_ParecerPropostaConvenioWSDTO
  public $permiteAdiarEntregaProjBasico; // boolean
  public $permiteLiberarPrimeiroRepasse; // boolean
  public $permiteProrrogarEntregaProjBasico; // boolean
  public $prazoEntregaProjBasico; // int
  public $proponenteWS; // ParticipantesPropostaWSDTO
  public $propostaParticipeMembroWS; // ArrayOf_tns1_ResponsaveisConvenioWSDTO
  public $propostaProgramaWS; // ArrayOf_tns1_PropostaProgramaWSDTO
  public $repasseExercicioFuturoPropostaWS; // ArrayOf_tns1_RepasseExercicioFuturoPropostaWSDTO
  public $sequencial; // int
  public $situacaoEnvioContratoRepasseWS; // SituacaoEnvioContratoRepasseWSDTO
  public $situacaoLegado; // int
  public $situacaoProjetoBasicoWS; // SituacaoProjetoBasicoWSDTO
  public $situacaoPropostaWS; // SituacaoPropostaWSDTO
  public $tipoNaoAcatamentoContratoRepasseWS; // TipoNaoAcatamentoContratoRepasseWSDTO
  public $tipoProjetoBasicoWS; // TipoProjetoBasicoWSDTO
  public $valorContraPartida; // double
  public $valorContrapartidaBensServicos; // double
  public $valorContrapartidaFinanceira; // double
  public $valorGlobal; // double
  public $valorRepasse; // double
}

class ConsultaConvenioDTO {
  public $anExercicio; // string
  public $coProgramaFnde; // string
  public $messages; // ArrayOf_xsd_string
}

class ConsultaEmpenhoDTO {
  public $anConvenio; // string
  public $anExercicio; // string
  public $nuConvenio; // string
  public $nuProcesso; // string
  public $nuPropostaSiconv; // string
}

class ConsultaEmpenhoResponseEntryDTO {
  public $anConvenio; // string
  public $anExercicio; // string
  public $coCentroGestaoAprov; // string
  public $coEspecieEmpenho; // string
  public $coFonteRecursoAprov; // string
  public $coGestaoEmitente; // string
  public $coNaturezaDespesaAprov; // string
  public $coPlanoInternoAprov; // string
  public $coProgramaFnde; // string
  public $coPtresAprov; // string
  public $coSituacaoDocSiafi; // string
  public $coTipoDocumento; // string
  public $coUnidadeGestoraEmitente; // string
  public $dsUsernameMovimento; // string
  public $dtMovimento; // string
  public $nuCgcFavorecido; // string
  public $nuConvenio; // string
  public $nuConvenioSiconv; // string
  public $nuEmpenhoOriginal; // string
  public $nuEmpenhoSiafi; // string
  public $nuIdSistema; // string
  public $nuProcesso; // string
  public $nuPropostaSiconv; // string
  public $nuSeqDocSiafi; // long
  public $nuSeqMovNe; // long
  public $vlEmpenho; // string
}

class ConsultaEmpenhoResponseDTO {
  public $consultaEmpenhoResponseEntryDTO; // ArrayOf_tns1_ConsultaEmpenhoResponseEntryDTO
  public $messages; // ArrayOf_xsd_string
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

class PropostaResponseDTO {
  public $mensagem; // ArrayOf_xsd_string
  public $numeroProposta; // string
  public $statusErro; // boolean
  public $timeline; // string
  public $tipoMensagem; // string
}


/**
 * SimecWsFacadeService class
 * 
 *  
 * 
 * @author    {author}
 * @copyright {copyright}
 * @package   {package}
 */
class SimecWsFacadeService extends SoapClient {

  private static $classmap = array(
                                    'enviarProposta' => 'enviarProposta',
                                    'enviarPropostaResponse' => 'enviarPropostaResponse',
                                    'consultarConvenio' => 'consultarConvenio',
                                    'consultarConvenioResponse' => 'consultarConvenioResponse',
                                    'ConsultaConvenioResponseDTO' => 'ConsultaConvenioResponseDTO',
                                    'consultarEmpenho' => 'consultarEmpenho',
                                    'consultarEmpenhoResponse' => 'consultarEmpenhoResponse',
                                    'solicitarNotaEmpenho' => 'solicitarNotaEmpenho',
                                    'solicitarNotaEmpenhoResponse' => 'solicitarNotaEmpenhoResponse',
                                    'UnidadeFederativaWSDTO' => 'UnidadeFederativaWSDTO',
                                    'MunicipioWSDTO' => 'MunicipioWSDTO',
                                    'NaturezaAquisicaoWSDTO' => 'NaturezaAquisicaoWSDTO',
                                    'NaturezaDespesaWSDTO' => 'NaturezaDespesaWSDTO',
                                    'NaturezaDespesaSubItemWSDTO' => 'NaturezaDespesaSubItemWSDTO',
                                    'ObjetoWSDTO' => 'ObjetoWSDTO',
                                    'CnpjBeneficiarioEspecificoWSDTO' => 'CnpjBeneficiarioEspecificoWSDTO',
                                    'CnpjProgramaEmendaWSDTO' => 'CnpjProgramaEmendaWSDTO',
                                    'OrigemRecursoPropProgramaWSDTO' => 'OrigemRecursoPropProgramaWSDTO',
                                    'MembroParticipanteWSDTO' => 'MembroParticipanteWSDTO',
                                    'AlteracaoEstatuariaWSDTO' => 'AlteracaoEstatuariaWSDTO',
                                    'AreaAtuacaoEntidadePrivadaWSDTO' => 'AreaAtuacaoEntidadePrivadaWSDTO',
                                    'RespostaCertidaoWSDTO' => 'RespostaCertidaoWSDTO',
                                    'CertidaoWSDTO' => 'CertidaoWSDTO',
                                    'CnaeWSDTO' => 'CnaeWSDTO',
                                    'DirigenteWSDTO' => 'DirigenteWSDTO',
                                    'DeclaracaoNaoDividaWSDTO' => 'DeclaracaoNaoDividaWSDTO',
                                    'EsferaAdministrativaWSDTO' => 'EsferaAdministrativaWSDTO',
                                    'EstatutoWSDTO' => 'EstatutoWSDTO',
                                    'LoginWSDTO' => 'LoginWSDTO',
                                    'TipoIdentificadorParticipeWSDTO' => 'TipoIdentificadorParticipeWSDTO',
                                    'ParticipantesPropostaWSDTO' => 'ParticipantesPropostaWSDTO',
                                    'ResponsaveisConvenioWSDTO' => 'ResponsaveisConvenioWSDTO',
                                    'OrgaoAdministrativoWSDTO' => 'OrgaoAdministrativoWSDTO',
                                    'ModalidadeConvenioWSDTO' => 'ModalidadeConvenioWSDTO',
                                    'NaturezaJuridicaWSDTO' => 'NaturezaJuridicaWSDTO',
                                    'RegrasWSDTO' => 'RegrasWSDTO',
                                    'ProgramaWSDTO' => 'ProgramaWSDTO',
                                    'QualificacaoProponenteWSDTO' => 'QualificacaoProponenteWSDTO',
                                    'PropostaProgramaWSDTO' => 'PropostaProgramaWSDTO',
                                    'StatusBensServicosPropostaWSDTO' => 'StatusBensServicosPropostaWSDTO',
                                    'BensServicosPropostaWSDTO' => 'BensServicosPropostaWSDTO',
                                    'BancoWSDTO' => 'BancoWSDTO',
                                    'ContaBancariaWSDTO' => 'ContaBancariaWSDTO',
                                    'ParcelaEtapaWSDTO' => 'ParcelaEtapaWSDTO',
                                    'ParcelaMetaWSDTO' => 'ParcelaMetaWSDTO',
                                    'TipoParticipanteWSDTO' => 'TipoParticipanteWSDTO',
                                    'DadosOrcamentariosWSDTO' => 'DadosOrcamentariosWSDTO',
                                    'OBConfluxoExportacaoWSDTO' => 'OBConfluxoExportacaoWSDTO',
                                    'EventoOrdemBancariaWSDTO' => 'EventoOrdemBancariaWSDTO',
                                    'OrdemBancariaExportacaoWSDTO' => 'OrdemBancariaExportacaoWSDTO',
                                    'ProgramacaoFinanceiraExportacaoWSDTO' => 'ProgramacaoFinanceiraExportacaoWSDTO',
                                    'TransferenciaRecursosWSDTO' => 'TransferenciaRecursosWSDTO',
                                    'ParcelaCronogramaDesembolsoWSDTO' => 'ParcelaCronogramaDesembolsoWSDTO',
                                    'CronogramaDesembolsoPropostaWSDTO' => 'CronogramaDesembolsoPropostaWSDTO',
                                    'EtapaCronogramaFisicoWSDTO' => 'EtapaCronogramaFisicoWSDTO',
                                    'MetaCronogramaFisicoWSDTO' => 'MetaCronogramaFisicoWSDTO',
                                    'CronogramaFisicoPropostaWSDTO' => 'CronogramaFisicoPropostaWSDTO',
                                    'TipoDespesaAdministrativaWSDTO' => 'TipoDespesaAdministrativaWSDTO',
                                    'DespesaAdministrativaWSDTO' => 'DespesaAdministrativaWSDTO',
                                    'ModalidadePropostaWSDTO' => 'ModalidadePropostaWSDTO',
                                    'MotivoNaoAcatamentoContratoRepasseWSDTO' => 'MotivoNaoAcatamentoContratoRepasseWSDTO',
                                    'ParecerPlanoTrabalhoWSDTO' => 'ParecerPlanoTrabalhoWSDTO',
                                    'ParecerPropostaConvenioWSDTO' => 'ParecerPropostaConvenioWSDTO',
                                    'RepasseExercicioFuturoPropostaWSDTO' => 'RepasseExercicioFuturoPropostaWSDTO',
                                    'SituacaoEnvioContratoRepasseWSDTO' => 'SituacaoEnvioContratoRepasseWSDTO',
                                    'SituacaoProjetoBasicoWSDTO' => 'SituacaoProjetoBasicoWSDTO',
                                    'SituacaoPropostaWSDTO' => 'SituacaoPropostaWSDTO',
                                    'TipoNaoAcatamentoContratoRepasseWSDTO' => 'TipoNaoAcatamentoContratoRepasseWSDTO',
                                    'TipoProjetoBasicoWSDTO' => 'TipoProjetoBasicoWSDTO',
                                    'PropostaWSDTO' => 'PropostaWSDTO',
                                    'ConsultaConvenioDTO' => 'ConsultaConvenioDTO',
                                    'ConsultaEmpenhoDTO' => 'ConsultaEmpenhoDTO',
                                    'ConsultaEmpenhoResponseEntryDTO' => 'ConsultaEmpenhoResponseEntryDTO',
                                    'ConsultaEmpenhoResponseDTO' => 'ConsultaEmpenhoResponseDTO',
                                    'EmpenhoDTO' => 'EmpenhoDTO',
                                    'PropostaResponseDTO' => 'PropostaResponseDTO',
                                   );

  public function SimecWsFacadeService($wsdl = "http://172.20.65.93:8080/IntraSiconvWS/services/SimecWsFacade?wsdl", $options = array()) {
    foreach(self::$classmap as $key => $value) {
      if(!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }
    parent::__construct($wsdl, $options);
  }

  /**
   *  
   *
   * @param enviarProposta $parameters
   * @return enviarPropostaResponse
   */
  public function enviarProposta(enviarProposta $parameters) {
    return $this->__soapCall('enviarProposta', array($parameters),       array(
            'uri' => 'http://simec.intraSiconvWS.sistema.fnde.gov.br',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarConvenio $parameters
   * @return consultarConvenioResponse
   */
  public function consultarConvenio(consultarConvenio $parameters) {
    return $this->__soapCall('consultarConvenio', array($parameters),       array(
            'uri' => 'http://simec.intraSiconvWS.sistema.fnde.gov.br',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param consultarEmpenho $parameters
   * @return consultarEmpenhoResponse
   */
  public function consultarEmpenho(consultarEmpenho $parameters) {
    return $this->__soapCall('consultarEmpenho', array($parameters),       array(
            'uri' => 'http://simec.intraSiconvWS.sistema.fnde.gov.br',
            'soapaction' => ''
           )
      );
  }

  /**
   *  
   *
   * @param solicitarNotaEmpenho $parameters
   * @return solicitarNotaEmpenhoResponse
   */
  public function solicitarNotaEmpenho(solicitarNotaEmpenho $parameters) {
    return $this->__soapCall('solicitarNotaEmpenho', array($parameters),       array(
            'uri' => 'http://simec.intraSiconvWS.sistema.fnde.gov.br',
            'soapaction' => ''
           )
      );
  }

}

?>
