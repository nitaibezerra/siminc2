<?php

require_once APPRAIZ . 'ted/classes/Ted/Model/Utils.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/TermoExecucaoDescentralizada.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/RepresentanteLegal.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/CoordenacaoResponsavel.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/Justificativa.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/PrevisaoOrcamentaria.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/Parecer.php';
require_once APPRAIZ . 'ted/classes/Ted/Model/Historico.php';

/**
 * Class Ted_Model_Historico_Commit
 */
class Ted_Model_Historico_Commit extends Ted_Model_Historico
{
    /**
     * Ted_Model_TermoExecucaoDescentralizada
     */
    protected $_modelTed;

    /**
     * @var
     */
    protected $_linha;

    /**
     * @var
     */
    protected $_version;

    /**
     *
     */
    public function __construct($tcpid)
    {
        $this->_tcpid = $tcpid;
        if (is_null($this->_tcpid)) {
            throw new Exception("Nenhum termo foi setado para a operação de histórico.");
        }

        $this->_modelTed = new Ted_Model_TermoExecucaoDescentralizada($this->_tcpid);
        $this->_version = (int) $this->_modelTed->capturaContagemTermo()+1;
    }

    /**
     * metodo que comita toda a operação de historico/versionamento do termo
     * @return void(0)
     */
    public function save()
    {
        $this->getTermo();
        if (!is_array($this->_linha)) return false;

        $this->salvarTermo();
        $this->salvarRepresentanteLegal();
        $this->salvarCoordenacaoResponsavel();
        $this->salvarJustificativa();
        $this->salvarPrevisaoOrcamentaria();
        $this->salvarPrevisaoParcela();
        $this->salvarCreditoRemanejado();
        $this->salvarParecerTecnico();
        $this->salvarArquivoPrevisaoOrcamentaria();
        $this->rcoAnexos();
        $this->salvarRCO();
        $this->ncRCO();
    }

    /**
     * Pega os dados padrões para qualquer termo
     * @return array|null
     */
    public function getTermo()
    {
        $strSQL = "
            SELECT
                tcpid, docid, ungcodproponente, ungcodconcedente, usucpfconcedente,
                usucpfproponente, cooid, entid, tcpobsrelatorio, ungcodpoliticafnde,
                dircodpoliticafnde, tcpnumtransfsiafi, tcpnumprocessofnde, tcpprogramafnde,
                tcpobsfnde, ungcodemitente, gescodemitente, tcpstatus,
                tcpobscomplemento, tcpbancofnde, tcpagenciafnde
            FROM ted.termocompromisso WHERE tcpid = %d
        ";

        $stmt = sprintf($strSQL, $this->_tcpid);
        $this->_linha = $this->pegaLinha($stmt);
    }

    /**
     * @return bool
     */
    public function salvarTermo()
    {
        extract($this->_linha);

        $entid = (is_null($entid)) ? 'null' : $entid;
        $dircodpoliticafnde = (is_null($dircodpoliticafnde)) ? 'null' : $dircodpoliticafnde;
        $ungcodemitente = (is_null($ungcodemitente)) ? 'null' : $ungcodemitente;
        $gescodemitente = (is_null($gescodemitente)) ? 'null' : $gescodemitente;
        $tcpobsrelatorio = addslashes($tcpobsrelatorio);
        $tcpobscomplemento = addslashes($tcpobscomplemento);
        $cooid = (!$cooid) ? 'null' : $cooid;
        $entid = (!$entid) ? 'null' : $entid;

        $strInsert = "
            INSERT INTO ted.historico_termocompromisso
            (tcpid, docid, ungcodproponente, ungcodconcedente, usucpfconcedente, usucpfproponente,
            cooid, entid, tcpobsrelatorio, ungcodpoliticafnde, dircodpoliticafnde, tcpnumtransfsiafi,
            tcpnumprocessofnde, tcpprogramafnde, tcpobsfnde, ungcodemitente,
            gescodemitente, tcpstatus, tcpobscomplemento, tcpbancofnde,
            tcpagenciafnde, tcpversion)
            VALUES
            ($tcpid, $docid, '$ungcodproponente', '$ungcodconcedente', '$usucpfconcedente', '$usucpfproponente',
            $cooid, $entid, '$tcpobsrelatorio', '$ungcodpoliticafnde', $dircodpoliticafnde, '$tcpnumtransfsiafi',
            '$tcpnumprocessofnde', '$tcpprogramafnde', '$tcpobsfnde', $ungcodemitente,
            $gescodemitente, '$tcpstatus', '$tcpobscomplemento', '$tcpbancofnde',
            '$tcpagenciafnde', $this->_version)
        ";

        $this->executar($strInsert);
        $this->commit();
        //echo $strInsert . '<br/>';
    }

    /**
     *
     */
    private function salvarRepresentanteLegal()
    {
        $rlt = new Ted_Model_RepresentanteLegal();

        //Representante legal do Proponente (titutar e substituto)
        $rlpTitular = $rlt->pegaResponsavelUG($this->_linha['ungcodproponente'], 'f');
        $rlpSubstituo = $rlt->pegaResponsavelUG($this->_linha['ungcodproponente'], 't');

        //Representante legal do Concedente (titular e substituto)
        $rlcTitular = $rlt->pegaResponsavelUG($this->_linha['ungcodconcedente'], 'f');
        $rlcSubstituto = $rlt->pegaResponsavelUG($this->_linha['ungcodconcedente'], 't');
        //ver($rlpTitular, $rlpSubstituo, $rlcTitular, $rlcSubstituto,d);

        if ($rlpTitular) {
            extract($rlpTitular);
            $strInsert = "
                INSERT INTO ted.historico_representantelegal(rlid, ug, cpf, nome, email, status, substituto, tcpversion, tcpid)
                VALUES ($rlid, '$ug', '$usucpf', '$usunome', '$usuemail', '$rpustatus', '$substituto', $this->_version, $this->_tcpid)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }

        if ($rlpSubstituo) {
            extract($rlpSubstituo);
            $strInsert = "
                INSERT INTO ted.historico_representantelegal(rlid, ug, cpf, nome, email, status, substituto, tcpversion, tcpid)
                VALUES ($rlid, '$ug', '$usucpf', '$usunome', '$usuemail', '$rpustatus', '$substituto', $this->_version, $this->_tcpid)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }

        if ($rlcTitular) {
            extract($rlcTitular);
            $strInsert = "
                INSERT INTO ted.historico_representantelegal(rlid, ug, cpf, nome, email, status, substituto, tcpversion, tcpid)
                VALUES ($rlid, '$ug', '$usucpf', '$usunome', '$usuemail', '$rpustatus', '$substituto', $this->_version, $this->_tcpid)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }

        if ($rlcSubstituto) {
            extract($rlcSubstituto);
            $strInsert = "
                INSERT INTO ted.historico_representantelegal(rlid, ug, cpf, nome, email, status, substituto, tcpversion, tcpid)
                VALUES ($rlid, '$ug', '$usucpf', '$usunome', '$usuemail', '$rpustatus', '$substituto', $this->_version, $this->_tcpid)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarCoordenacaoResponsavel()
    {
        $cr = new Ted_Model_CoordenacaoResponsavel($this->_tcpid);
        $proponente = $cr->get($this->_linha['ungcodproponente']);
        $concedente = $cr->get($this->_linha['ungcodconcedente']);

        if ($proponente) {
            extract($proponente);
            $strInsert = "
                INSERT INTO ted.historico_coordenacao_responsavel(corid, tcpid, ungcod, nomecoordenacao, dddcoordenacao, telefonecoordenacao, datainsert, tcpversion)
                VALUES ('$corid', '$tcpid', '$ungcod', '$nomecoordenacao', '$dddcoordenacao', '$telefonecoordenacao', '$datainsert', $this->_version)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }

        if ($concedente) {
            extract($concedente);
            $strInsert = "
                INSERT INTO ted.historico_coordenacao_responsavel(corid, tcpid, ungcod, nomecoordenacao, dddcoordenacao, telefonecoordenacao, datainsert, tcpversion)
                VALUES ('$corid', '$tcpid', '$ungcod', '$nomecoordenacao', '$dddcoordenacao', '$telefonecoordenacao', '$datainsert', $this->_version)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarJustificativa()
    {
        $model = new Ted_Model_Justificativa($this->_tcpid);
        $justificativa = $model->capturaDadosJustificativa();
        //ver($justificativa);

        if ($justificativa) {
            extract($justificativa);

            $identificacao = addslashes($identificacao);
            $objetivo = addslashes($objetivo);
            $justificativa = addslashes($justificativa);

            $strInsert = "
                INSERT INTO ted.historico_justificativa(justid, identificacao, objetivo, justificativa, tcpid, tcpversion)
                VALUES ($justid, '$identificacao', '$objetivo', '$justificativa', $this->_tcpid, $this->_version)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarPrevisaoOrcamentaria()
    {
        $model = new Ted_Model_PrevisaoOrcamentaria($this->_tcpid);
        $previsoes = $model->getPrevisoes();
        //ver($previsoes, d);

        if (is_array($previsoes)) {
            $strInsert = "
                INSERT INTO ted.historico_previsaoorcamentaria(proid, tcpid, ptrid, pliid, prodsc, ndpid,
                provalor, prodata, prostatus, crdmesliberacao, crdmesexecucao, proanoreferencia,
                prgidfnde, prgfonterecurso, espid, esfid, creditoremanejado, tcpversion) VALUES
            ";

            foreach ($previsoes as $previsao) {
                extract($previsao);

                $crdmesliberacao = (is_null($crdmesliberacao)) ? 'null' : $crdmesliberacao;
                $crdmesexecucao = (is_null($crdmesexecucao)) ? 'null' : $crdmesexecucao;
                $proanoreferencia = (is_null($proanoreferencia)) ? 'null' : $proanoreferencia;
                $prgidfnde = (is_null($prgidfnde)) ? 'null' : $prgidfnde;
                $espid = (is_null($espid)) ? 'null' : $espid;
                $esfid = (is_null($esfid)) ? 'null' : $esfid;
                $creditoremanejado = (is_null($creditoremanejado)) ? 'null' : "'$creditoremanejado'";
                $ptrid = (is_null($ptrid)) ? 'null' : $ptrid;
                $pliid = (is_null($pliid)) ? 'null' : $pliid;

                $strInsert .= "($proid, $this->_tcpid, $ptrid, $pliid, '$prodsc', $ndpid, $provalor, '$prodata', '$prostatus', $crdmesliberacao, $crdmesexecucao, $proanoreferencia, $prgidfnde, '$prgfonterecurso', $espid, $esfid, $creditoremanejado, $this->_version),";
            }

            $strInsert = substr($strInsert, 0, -1);
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarPrevisaoParcela()
    {
        $strSQL = "
            SELECT * FROM ted.previsaoparcela WHERE proid IN (
                SELECT proid FROM ted.previsaoorcamentaria WHERE tcpid = {$this->_tcpid}
            )
        ";
        $parcelas = $this->carregar($strSQL);
        //ver($parcelas, d);
        if (is_array($parcelas)) {
            $strInsert = "
                INSERT INTO ted.historico_previsaoparcela(ppaid, proid, ppavlrparcela, codsigefnc, tcpnumtransfsiafi,
                ppadata, ppacancelarnc, ppamesenvio, ppanumeromacro, codncsiafi, ppanumcancelanc,
                ppaultimoretornosigef, ppacadastradosigef, tcpversion, tcpid) VALUES
            ";

            foreach ($parcelas as $parcela) {
                extract($parcela);

                $codsigefnc = (is_null($codsigefnc)) ? 'null' : $codsigefnc;
                $ppamesenvio = (is_null($ppamesenvio)) ? 'null' : $ppamesenvio;
                $ppanumeromacro = (is_null($ppanumeromacro)) ? 'null' : $ppanumeromacro;
                $ppacancelarnc = (is_null($ppacancelarnc)) ? 'null' : "'$ppacancelarnc'";
                $ppanumcancelanc = (is_null($ppanumcancelanc)) ? 'null' : "'$ppanumcancelanc'";

                $strInsert.= "($ppaid, $proid, $ppavlrparcela, $codsigefnc, '$tcpnumtransfsiafi', '$ppadata', $ppacancelarnc, $ppamesenvio, $ppanumeromacro, '$codncsiafi', '$ppanumcancelanc', '$ppaultimoretornosigef', '$ppacadastradosigef', $this->_version, $this->_tcpid),";
            }

            $strInsert = substr($strInsert, 0, -1);
            //echo $strInsert . '<br/>'; die;
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     * @return bool
     */
    public function salvarCreditoRemanejado()
    {
        $strSQL = sprintf("
            select * from ted.creditoremanejado where proid in (
                select proid from ted.previsaoorcamentaria where tcpid = %d
            )
        ", $this->_tcpid);
        $creditos = $this->carregar($strSQL);
        if (!$creditos) return false;

        $strInsert = "INSERT INTO ted.historico_creditoremanejado(crid, proid, valor, nc_devolucao, crdata, crobservacao, usucpf, tcpversion, tcpid) VALUES ";

        foreach ($creditos as $credito) {
            extract($credito);
            $crobservacao = addslashes($crobservacao);
            $strInsert .= "($crid, $proid, $valor, '$nc_devolucao', '$crdata', '$crobservacao', '$usucpf', $this->_version, $this->_tcpid),";
        }

        $strInsert = substr($strInsert, 0, -1);
        //echo $strInsert . '<br/>';
        $this->executar($strInsert);
        $this->commit();
    }

    /**
     *
     */
    public function salvarParecerTecnico()
    {
        $model = new Ted_Model_Parecer($this->_tcpid);
        $parecer = $model->capturaDadosParecerTecnico();
        //ver($parecer, d);

        if (is_array($parecer)) {
            extract($parecer);

            $considentproponente = addslashes($considentproponente);
            $considproposta = addslashes($considproposta);
            $considobjeto = addslashes($considobjeto);
            $considobjetivo = addslashes($considobjetivo);
            $considjustificativa = addslashes($considjustificativa);
            $considvalores = addslashes($considvalores);
            $considcabiveis = addslashes($considcabiveis);

            $strInsert = "
                INSERT INTO ted.historico_parecertecnico(ptecid, considentproponente, considproposta, considobjeto,
                considobjetivo, considjustificativa, considvalores, considcabiveis, usucpfparecer, tcpid, tcpversion)
                VALUES ($ptecid, '$considentproponente', '$considproposta', '$considobjeto', '$considobjetivo',
                '$considjustificativa', '$considvalores', '$considcabiveis', '$usucpfparecer',
                $this->_tcpid, $this->_version)
            ";

            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarArquivoPrevisaoOrcamentaria()
    {
        $strSQL = sprintf("
            select * from ted.arquivoprevorcamentaria where tcpid = %d;
        ", $this->_tcpid);
        $arquivos = $this->carregar($strSQL);
        //ver($arquivos, d);

        if (is_array($arquivos)) {
            $strInsert = "INSERT INTO ted.historico_arquivoprevorcamentaria(arqid, tcpid, arpdtinclusao, arpstatus, arpdsc, arptipo, tcpversion) VALUES ";

            foreach ($arquivos as $arquivo) {
                extract($arquivo);
                $strInsert .= "($arqid, $this->_tcpid, '$arpdtinclusao', '$arpstatus', '$arpdsc', '$arptipo', $this->_version),";
            }

            $strInsert = substr($strInsert, 0, -1);
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     *
     */
    private function salvarRCO()
    {
        $strSQL = sprintf("
            SELECT * FROM ted.relatoriocumprimento WHERE recstatus = 'A' AND tcpid = %d
        ", $this->_tcpid);
        $rco = $this->pegaLinha($strSQL);

        if (is_array($rco)) {
            extract($rco);

            $recatividadesprevistas = addslashes($recatividadesprevistas);
            $recmetaprevista = addslashes($recmetaprevista);
            $recatividadesexecutadas = addslashes($recatividadesexecutadas);
            $recmetaexecutada = addslashes($recmetaexecutada);
            $recdificuldades = addslashes($recdificuldades);
            $recmetasadotadas = addslashes($recmetasadotadas);
            $reccomentarios = addslashes($reccomentarios);
            $recdtpublicacao = (empty($recdtpublicacao)) ? 'null' : "'{$recdtpublicacao}'";
            $recdtemissaorgresposavel = (empty($recdtemissaorgresposavel)) ? 'null' : "'$recdtemissaorgresposavel'";

            $strInsert = "
                INSERT INTO ted.historico_relatoriocumprimento
                (recid,
                 tcpid,
                 reccnpj,
                 recnome,
                 recendereco,
                 muncod,
                 estuf,
                 reccep,
                 rectelefone,
                 uocod,
                 ugcod,
                 gestaocod,
                 recnomeresponsavel,
                 reccpfresponsavel,
                 recsiaperesponsavel,
                 recrgresponsavel,
                 recdtemissaorgresposavel,
                 recexpedidorrgresposavel,
                 reccargo,
                 recemailresposavel,
                 recnumportaria,
                 recdtpublicacao,
                 recnumnotacredito,
                 recexecucaoobjeto,
                 recatividadesprevistas,
                 recmetaprevista,
                 recatividadesexecutadas,
                 recmetaexecutada,
                 recdificuldades,
                 recmetasadotadas,
                 reccomentarios,
                 recvlrrecebido,
                 recvlrutilizado,
                 recvlrdevolvido,
                 recnumncdevolucao,
                 recstatus,
                 tcpversion)
                 VALUES
                 ($recid,
                 $this->_tcpid,
                 '$reccnpj',
                 '$recnome',
                 '$recendereco',
                 '$muncod',
                 '$estuf',
                 '$reccep',
                 '$rectelefone',
                 '$uocod',
                 '$ugcod',
                 '$gestaocod',
                 '$recnomeresponsavel',
                 '$reccpfresponsavel',
                 '$recsiaperesponsavel',
                 '$recrgresponsavel',
                 $recdtemissaorgresposavel,
                 '$recexpedidorrgresposavel',
                 '$reccargo',
                 '$recemailresposavel',
                 '$recnumportaria',
                 $recdtpublicacao,
                 '$recnumnotacredito',
                 '$recexecucaoobjeto',
                 '$recatividadesprevistas',
                 '$recmetaprevista',
                 '$recatividadesexecutadas',
                 '$recmetaexecutada',
                 '$recdificuldades',
                 '$recmetasadotadas',
                 '$reccomentarios',
                 '$recvlrrecebido',
                 '$recvlrutilizado',
                 '$recvlrdevolvido',
                 '$recnumncdevolucao',
                 '$recstatus',
                 $this->_version)
            ";

            $this->executar($strInsert);
            $this->commit();
        }
    }

    /**
     * @return bool
     */
    private function ncRCO()
    {
        $strSQL = sprintf("
            select * from ted.ncrelatoriocumprimento where tcpid = %d;
        ", $this->_tcpid);
        $ncRco = $this->carregar($strSQL);
        //ver($ncRco, d);
        if (!is_array($ncRco)) return false;

        foreach ($ncRco as $row) {
            extract($row);
            $strInsert = "
                INSERT INTO ted.historico_ncrelatoriocumprimento
                (rcnid, tcpid, recid, rcnnumnc, rcndevolucao, rpustatus, tcpversion)
                VALUES ($rcnid, $this->_tcpid, $recid, '$rcnnumnc', '$rcndevolucao', '$rpustatus', $this->_version)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
        }

        $this->commit();
    }

    /**
     * @return bool
     */
    private function rcoAnexos()
    {
        $strSQL = sprintf("
            select * from ted.relatoriocumprimentoanexo where recid in (
                select recid from ted.ncrelatoriocumprimento where tcpid = %d
            )
        ", $this->_tcpid);
        $anexos = $this->carregar($strSQL);
        //ver($anexos, d);
        if (!is_array($anexos)) return false;

        foreach ($anexos as $anexo) {
            extract($anexo);
            $strInsert = "
                INSERT INTO ted.historico_relatoriocumprimentoanexo
                (anxid, arqid, recid, usucpf, tcpversion, tcpid) VALUES
                ($anxid, $arqid, $recid, '$usucpf', $this->_version, $this->_tcpid)
            ";
            //echo $strInsert . '<br/>';
            $this->executar($strInsert);
        }

        $this->commit();
    }
}