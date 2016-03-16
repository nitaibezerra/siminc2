<?php

class AuditoriaRepositorio extends Modelo {

    private $fiscalRepositorio;
    private $analiseSolicitacaoRepositorio;
    private $situacaoAuditoriaRepositorio;
    private $tipoDocumentoRepositorio;
    private $estadoDocumentoRepositorio;

    function __construct() {
        parent::__construct();
        $this->fiscalRepositorio = new FiscalRepositorio();
        $this->analiseSolicitacaoRepositorio = new AnaliseSolicitacaoRepositorio();
        $this->situacaoAuditoriaRepositorio = new SituacaoAuditoriaRepositorio();
        $this->tipoDocumentoRepositorio = new TipoDocumentoRepositorio();
        $this->estadoDocumentoRepositorio = new EstadoDocumentoRepositorio();
    }

    function salvar(Auditoria $auditoria) {
        if ($auditoria->getId() == '' || $auditoria->getId() == null) {
            $fiscal  = $auditoria->getFiscal();
            $idAnalise = $auditoria->getAnaliseSolicitacao()->getId();
            $cpfUsuario =  !empty( $fiscal ) ? $fiscal->getId() : NULL;
            $responsavel = $auditoria->getNomeResponsavelFabrica();
            //$situacao = $auditoria->getSituacaoAuditoria()->getId();
            $docid = $this->criaDocumentoPendenteEmWorkflow();

            $sql = "INSERT INTO fabrica.auditoria (ansid, usucpf, audrespfabrica, docid)
					VALUES ($idAnalise, '$cpfUsuario', '$responsavel', $docid) returning audid";

            $novoId = parent::pegaUm($sql);
            parent::commit();
            return $novoId;
        } else {
            $id = $auditoria->getId();
            $idAnaliseSolicitacao = $auditoria->getAnaliseSolicitacao()->getId();
            $idFiscal = $auditoria->getFiscal()->getId();
            $nomeResponsavel = $auditoria->getNomeResponsavelFabrica() != '' ? "'" . $auditoria->getNomeResponsavelFabrica() . "'" : "null";
            $idSituacaoAuditoria = $auditoria->getSituacaoAuditoria() != '' ? $auditoria->getSituacaoAuditoria()->getId() : "null";
            $idDocumento = $auditoria->getIdDocumento() != '' ? $auditoria->getIdDocumento() : "null";

            $sql = "UPDATE fabrica.auditoria SET ansid = $idAnaliseSolicitacao,
				usucpf = '$idFiscal', audrespfabrica = $nomeResponsavel, 
				audsituacao = $idSituacaoAuditoria WHERE audid = $id";

            return parent::executar($sql) && parent::commit();
        }
    }

    public function criaDocumentoPendenteEmWorkflow() {
        $tipoDocumento = $this->tipoDocumentoRepositorio->recuperePelaDescricao(TipoDocumento::ARTEFATOS);
        $idTipoDocumento = $tipoDocumento->getId();

        $estadoDocumento = $this->estadoDocumentoRepositorio->recuperePorTipoDocumentoEDescricao($idTipoDocumento, EstadoDocumento::ARTEFATO_PENDENTE);
        $idEstadoDocumento = $estadoDocumento->getId();

        $sql = "insert into workflow.documento
				( tpdid, esdid, docdsc )
				values ( $idTipoDocumento, $idEstadoDocumento, 'Documento Pendente' )
				returning docid";

        $docid = parent::pegaUm($sql);
        parent::commit();
        return $docid;
    }

    function recuperePeloIdSolicitacao($idSolicitacao) {
        $sql = "select
					aud.audid, aud.ansid, aud.usucpf, aud.audrespfabrica, aud.audsituacao, aud.docid 
				from fabrica.auditoria aud
				join fabrica.analisesolicitacao ans
					on ans.ansid = aud.ansid
				where ans.scsid = $idSolicitacao";

        $linha = parent::pegaLinha($sql);
        if ($linha != null) {
            $auditoria = $this->montaObjeto($linha);
            return $auditoria;
        } else {
            return null;
        }
    }

    function recuperePeloIdAnaliseSolicitacao($idAnaliseSolicitacao) {
        $sql = "select
					aud.audid, aud.ansid, aud.usucpf, aud.audrespfabrica, aud.audsituacao, aud.docid 
				from fabrica.auditoria aud
				join fabrica.analisesolicitacao ans
					on ans.ansid = aud.ansid
				where ans.ansid = $idAnaliseSolicitacao";

        $linha = parent::pegaLinha($sql);
        if ($linha != null) {
            $auditoria = $this->montaObjeto($linha);
            return $auditoria;
        } else {
            return null;
        }
    }

    function recuperePorId($idAuditoria) {
        $sql = "select aud.audid, aud.ansid, aud.usucpf, aud.audrespfabrica, aud.audsituacao, aud.docid 
				from fabrica.auditoria aud where aud.audid = $idAuditoria";

        $linha = parent::pegaLinha($sql);
        if ($linha != null) {
            $auditoria = $this->montaObjeto($linha);
            return $auditoria;
        } else {
            return null;
        }
    }

    private function montaObjeto($linha) {
        $auditoria = new Auditoria();
        $auditoria->setId($linha['audid']);

        $analiseSolicitacao = new AnaliseSolicitacao();
        $analiseSolicitacao->setId($linha['ansid']);
        $auditoria->setAnaliseSolicitacao($analiseSolicitacao);

        $auditoria->setFiscal($this->fiscalRepositorio->recuperePorId($linha['usucpf']));
        $auditoria->setNomeResponsavelFabrica($linha['audrespfabrica']);
        $auditoria->setSituacaoAuditoria($this->situacaoAuditoriaRepositorio->recuperePeloId($linha['audsituacao']));
        $auditoria->setIdDocumento($linha['docid']);
        return $auditoria;
    }

}