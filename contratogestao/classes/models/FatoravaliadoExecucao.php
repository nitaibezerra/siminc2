<?php

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

class Model_FatoravaliadoExecucao extends Model_Fatoravaliado
{

    function getConsultaExecucao($tipoFatorAvaliado)
    {
        $idSistema = ID_SISTEMA;
        $idTipoWorkflow = WF_CONTRATO_GESTAO;
        $cpf = $_SESSION['usucpf'];
        $dados = array();
        switch ($tipoFatorAvaliado) {
            case self::ESTADO_EXECUTOR:
                $campoPessoaFisica = 'usucpfexecutor';
                $campoPessoaJuridica = 'entidexecutor';
                $idPerfilFisica = CONTRATO_PERFIL_EXECUTOR;
                $idWorkflow = WF_CONTRATO_GESTAO_EXECUTACAO;
                $perfilJuridica = FUNCAO_EXECUTOR;
                break;

            case self::ESTADO_VALIDADOR:
                $campoPessoaFisica = 'usucpfvalidador';
                $campoPessoaJuridica = 'entidvalidador';
                $idPerfilFisica = CONTRATO_PERFIL_VALIDADOR;
                $idWorkflow = WF_CONTRATO_GESTAO_VALIDACAO;
                $perfilJuridica = FUNCAO_VALIDADOR;
                break;

            case self::ESTADO_CERTIFICADOR:
                $campoPessoaFisica = 'usucpfcertificador';
                $campoPessoaJuridica = 'entidcertificador';
                $idPerfilFisica = CONTRATO_PERFIL_CERTIFICADOR;
                $idWorkflow = WF_CONTRATO_GESTAO_CERTIFICACAO;
                $perfilJuridica = FUNCAO_CERTIFICADOR;
                break;
        }
        $parans = array('campoPessoaFisica' => $campoPessoaFisica, 'idPerfilFisica' => $idPerfilFisica, 'idSistema' => $idSistema,
            'cpf' => $cpf, 'idTipoWorkflow' => $idTipoWorkflow, 'idWorkflow' => $idWorkflow, 'perfilJuridica' => $perfilJuridica, 'campoPessoaJuridica' => $campoPessoaJuridica);

        if ($this->getAttributeValue('conid')) {
            $contrato = new Model_Contrato();
            $contratoArray = $contrato->getContratoById($this->getAttributeValue('conid'));
            $parans['conid'] = $contratoArray['conid'];
            $parans['hqcid'] = $contratoArray['hqcid'];
        }

        $dadosFisica = $this->getSqlPessoaFisica($parans);
        $dadosJuridica = $this->getSqlPessoaJuridica($parans);
        if ($dadosFisica) {
            $dados = $dadosFisica;
        }

        if ($dadosJuridica) {
            $dados = $dadosJuridica;
        }

        if (is_array($dadosFisica) && is_array($dadosJuridica)) {
            $dados = array_merge($dadosFisica, $dadosJuridica);
        }

        $dados = $this->adicionarHistorico($dados);
        usort($dados, array($this, 'date_compare'));
        return $dados;
    }

    function date_compare($a, $b)
    {
        $t1 = $this->geraTimestamp($a['fatprazo']);
        $t2 = $this->geraTimestamp($b['fatprazo']);
        return $t1 - $t2;
    }

    public function getListingExecucao()
    {
        $listing = new Listing(false);
        $listing->setPageNumber(50);
        $listing->setHead(array('Histórico', 'Data', 'Fator Avaliado'));
        $listing->setActions(array('edit' => 'editar'));
        return $listing;
    }

    public function setArqId($descricao = "Termo de Pré-Adesão", $campo = "arqid")
    {
        $file = new FilesSimec($this->_name, null, $this->_schema);
        $file->setUpload($descricao, $campo, false, false);
        $arqid = (int)$file->getIdArquivo();
        $this->setAttributeValue('arqid', $arqid);
    }

    public function getArquivo($idArquivo)
    {
        $file = new FilesSimec();
        $file->getDownloadArquivo($idArquivo);
    }

    private function getDadosExecucao($sql)
    {
        $hierarquiaContrato = new Model_Hierarquiacontrato();

        try {
            $data = $this->_db->carregar($sql);
        } catch (Exception $exc) {
            if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
                echo $exc->getTraceAsString();
            }
        }
        if ($data) {
            foreach ($data as $key => $valores) {
                foreach ($valores as $indice => $valor) {

                    if ($indice === 'hqcidpai' && !empty($valor)) {
                        $data[$key]['fatdsc'] = '<small style="font-size:8pt; color:#627384;">' . $hierarquiaContrato->getEnderecoArvoreContrato($valor, $data[$key]['hqcid']) . '</small> <br> ' . $data[$key]['fatdsc'];
                        unset($data[$key]['hqcidpai']);
                        unset($data[$key]['hqcid']);
                    }
//
                    if ($indice === 'fatprazo' && !empty($valor)) {
                        $data[$key][$indice] = date('d/m/Y', strtotime($valor));
                        $valor = date('d/m/Y', strtotime($valor));
                    }

                    if ($indice === 'fatprazo' && !empty($valor) && $this->dataEmAtraso($valor)) {
                        $data[$key][$indice] = $valor . '<br> <span class="label label-danger">em atraso</span> ';
                    }
                    if ($indice === 'fatprazo' && !empty($valor) && $this->dataEmHoje($valor)) {
                        $data[$key][$indice] = $valor . '<br> <span class="label label-success">hoje</span> ';
                    }
                }
            }
        }
        return $data;
    }

    public function getAcao($acao)
    {
        $codAcao = false;
        $entidvalidador = $this->getAttributeValue('entidvalidador');
        $usucpfvalidador = $this->getAttributeValue('usucpfvalidador');

        $entidcertificador = $this->getAttributeValue('entidcertificador');
        $usucpfcertificador = $this->getAttributeValue('usucpfcertificador');

        switch ($acao) {
            case 'execucao':
                if (!empty($entidvalidador) or !empty($usucpfvalidador)) {
                    $codAcao = WF_ACAO_EXECUTACAO_EXECUTADO;
                }
//                else {
//                    $codAcao = WF_ACAO_EXECUTACAO_FINALIZADO;
//                }
                break;
            case 'validacao':
                if (!empty($entidcertificador) or !empty($usucpfcertificador)) {
                    $codAcao = WF_ACAO_VALIDACAO_VALIDADO;
                } else {
                    $codAcao = WF_ACAO_VALIDACAO_FINALIZADO;
                }
                break;
            case 'certificacao':
                $codAcao = WF_ACAO_CERTIFICACAO_CERTIFICADO;
                break;
        }
        return $codAcao;
    }

    public function getAcaoRecusado($acao, $retorno)
    {
        $codAcao = false;
        switch ($acao) {
            case 'validacao':
                $codAcao = WF_ACAO_VALIDACAO_INVALIDADO;
                break;
            case 'certificacao':
                if ($retorno == 'validacao') {
                    $codAcao = WF_ACAO_CERTIFICACAO_VALIDACAO_NAO_CERTIFICADO;
                } else if ($retorno == 'execucao') {
                    $codAcao = WF_ACAO_CERTIFICACAO_EXECUCAO_NAO_CERTIFICADO;
                }
                break;
        }
        return $codAcao;
    }

    public function dataEmAtraso($time_inicial)
    {
        if (strpos($time_inicial, '/') !== false) {
            $time_inicial = $this->geraTimestamp($time_inicial);
        } else {
            $time_inicial = strtotime($time_inicial);
        }
        $time_final = strtotime(date('Y-m-d'));
        $diferenca = $time_final - $time_inicial;
        return ($diferenca > 0);
    }

    public function dataEmHoje($time_inicial)
    {
        if (strpos($time_inicial, '/') !== false) {
            $time_inicial = $this->geraTimestamp($time_inicial);
        } else {
            $time_inicial = strtotime($time_inicial);
        }
        $time_final = strtotime(date('Y-m-d'));
        if ($time_final == $time_inicial) {
            return true;
        } else {
            return false;
        }
    }

    function geraTimestamp($data)
    {
        $partes = explode('/', $data);
        return mktime(0, 0, 0, $partes[1], $partes[0], $partes[2]);
    }

    private function getSqlPessoaFisica(array $parans)
    {
        $sql = "SELECT 	fator.fatid, fator.docid as docid_, hierarquiacontrato.hqcidpai, hierarquiacontrato.hqcid, fatprazo, fator.fatdsc
	 
                FROM seguranca.usuario usu
                INNER JOIN seguranca.usuario_sistema ususistema ON ususistema.usucpf = usu.usucpf
                INNER JOIN seguranca.perfilusuario perfilusu ON perfilusu.usucpf = usu.usucpf
                INNER JOIN contratogestao.fatoravaliado fator ON fator.{$parans['campoPessoaFisica']} = usu.usucpf
                INNER JOIN workflow.documento documento ON documento.docid = fator.docid
                INNER JOIN workflow.estadodocumento estadodocumento ON estadodocumento.esdid = documento.esdid    
                INNER JOIN contratogestao.contrato as contrato ON fator.conid = contrato.conid 
                INNER JOIN contratogestao.hierarquiacontrato as hierarquiacontrato ON hierarquiacontrato.hqcid = contrato.hqcid
                 
                WHERE  
                        fator.fatstatus = 'A'
                        AND perfilusu.pflcod = {$parans['idPerfilFisica']}
                        AND ususistema.sisid = {$parans['idSistema']}
                        AND usu.usucpf = '{$parans['cpf']}'
                        AND estadodocumento.tpdid = {$parans['idTipoWorkflow']}
                        AND estadodocumento.esdid = {$parans['idWorkflow']} 
                ";

        if ($parans['conid']) {
            $itensContratos = $this->getItensContrato($parans['hqcid'], 7);
            if ($itensContratos) {
                foreach ($itensContratos as $value) {
                    $conIds[] = $value['conid'];
                }
                $strInArray = implode(',', $conIds);
                $sql .= " AND fator.conid in ( {$strInArray} ) ";
            } else {
                return false;
            }
        }

        $sql .= " ORDER BY fator.fatprazo ASC ";

//      ver($sql, d);
        return $this->getDadosExecucao($sql);
    }

    private function getSqlPessoaJuridica($parans)
    {
        $sql = "SELECT DISTINCT ON (fator.fatid) fator.fatid, fator.docid as docid_, hierarquiacontrato.hqcidpai, hierarquiacontrato.hqcid, fatprazo, fator.fatdsc
	FROM contratogestao.fatoravaliado fator
	INNER JOIN contratogestao.contrato as contrato ON fator.conid = contrato.conid 
        INNER JOIN contratogestao.hierarquiacontrato as hierarquiacontrato ON hierarquiacontrato.hqcid = contrato.hqcid
	INNER JOIN workflow.documento documento ON documento.docid = fator.docid
        INNER JOIN workflow.estadodocumento estadodocumento ON estadodocumento.esdid = documento.esdid    
                
	INNER JOIN entidade.entidade as entidade ON entidade.entid = fator.{$parans['campoPessoaJuridica']} 
        INNER JOIN contratogestao.usuarioresponsabilidade as usu_resp ON usu_resp.entid = entidade.entid 
                
        WHERE  
                fator.fatstatus = 'A'
                AND usu_resp.usucpf =  '{$parans['cpf']}'
                AND estadodocumento.tpdid = {$parans['idTipoWorkflow']}
                AND estadodocumento.esdid = {$parans['idWorkflow']} ";

        if ($parans['conid']) {
            $itensContratos = $this->getItensContrato($parans['hqcid'], 7);
            if ($itensContratos) {
                foreach ($itensContratos as $value) {
                    $conIds[] = $value['conid'];
                }
                $strInArray = implode(',', $conIds);
                $sql .= " AND fator.conid in ( {$strInArray} ) ";
            }
        }
        $sql .= " ORDER BY fator.fatid ";
//              ver($sql, d);
        return $this->getDadosExecucao($sql);
    }

    public function getItensContrato($hqcid = null, $nivel = 7)
    {
        $hierarquiacontrato = new Model_Hierarquiacontrato();
        $dados = $hierarquiacontrato->getNos($hqcid, " AND (q.h).hqcnivel = {$nivel} ");
        return $dados;
    }

}
