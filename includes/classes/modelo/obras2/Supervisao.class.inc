<?php

include_once APPRAIZ . 'includes/classes/modelo/obras2/SituacaoAtividade.class.inc';
include_once APPRAIZ . 'includes/classes/modelo/obras2/RegistroAtividade.class.inc';

class Supervisao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("supid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'supid'                         => null,
        'qlbid'                         => null,
        'dcnid'                         => null,
        'staid'                         => null,
        'rsuid'                         => null,
        'stpid'                         => null,
        'usucpf'                        => null,
        'docid'                         => null,
        'obrid'                         => null,
        'emsid'                         => null,
        'sueid'                         => null,
        'smiid'                         => null,
        'sfndeid'                       => null,
        'arqidplanilha'                 => null,
        'arqidtermo'                    => null,
        'supdata'                       => null,
        'supparecerorgao'               => null,
        'suppercprojexecutado'          => null,
        'supobs'                        => null,
        'supprojespecificacoes'         => null,
        'supplacaobra'                  => null,
        'supplacalocalterreno'          => null,
        'supvalidadealvara'             => null,
        'supstatus'                     => null,
        'supdtinclusao'                 => null,
        'supdiarioobra'                 => null,
        'suprealizacao'                 => null,
        'supvistoriador'                => null,
        'supdatafimobra'                => null,
        'supid_1'                       => null,
        'validadaPeloSupervisorUnidade' => null,
        'arquid_art_exec'               => null,
        'arquid_art_fisc'               => null,
        'arqidosexecucao'               => null,
        'croid'                         => null,
        'supjusticativaperc'            => null,
        'posvigencia'                   => null
    );
    
    protected $createVistoria;
    public $percentExec;

    public function pegaPercentSupervisao($supid) {
        $sql = "SELECT
                        COALESCE(SUM(si.spivlritemsobreobraexecanterior), 0) AS percentual
                FROM
                        obras2.supervisaoitem si
                JOIN obras2.supervisao su ON su.supid = si.supid
                JOIN
                        obras2.itenscomposicaoobra ico ON ico.icoid = si.icoid
                JOIN obras2.cronograma cro ON cro.croid = ico.croid ANd cro.crostatus IN ('A', 'H') AND cro.croid = su.croid
                WHERE
                        si.icoid IS NOT NULL AND
                        ico.relativoedificacao = 'D' AND
                        si.supid = {$supid}";
        $percentual = $this->pegaUm($sql);

        return $percentual;
    }

    public function listaDados($obrid, Array $param = array()) {
        $arWhere = array();

        if ($param['not(emsid)']) {
            $arWhere[] = "s.emsid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "s.smiid IS NULL";
        }

        $sql = "SELECT distinct
                                to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                                to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,
                                COALESCE('0', '-') AS aditivo,
                                u.usunome,
                                si.stadesc,
                                rs.rsudsc as responsavel,
                                UPPER(e.entnome) as vistoriador,
                                s.supid,
                                s.usucpf,
                                e.entnumcpfcnpj as cpfvistoriador,
                                s.supdtinclusao,
                                s.croid,
                                cro.crostatus,
                                ( SELECT
                                        CASE 
                                            WHEN SUM(icovlritem) > 0 THEN 
                                                ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                                            ELSE 
                                                0	
                                        END AS total
                                                -- SUM(spivlritemsobreobraexecanterior) AS total
                                    FROM
                                        obras2.itenscomposicaoobra i
                                    LEFT JOIN
                                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                                        AND sic.supid = s.supid
                                        AND sic.icoid IS NOT NULL
                                        AND sic.ditid IS NULL
                                    JOIN obras2.supervisao su ON su.supid = sic.supid
                                    JOIN obras2.cronograma cro ON cro.croid = su.croid AND cro.crostatus IN ('A', 'H')
                                    WHERE
                                        i.icostatus = 'A' AND
                                        i.relativoedificacao = 'D' AND
                                        i.obrid = '" . $obrid . "'
                                ) as percentual,
                                doc.esdid,
                                s.supdata,
                                s.supid_1,
                                rs.rsuid as realizacaosupervisao_rsuid
                            FROM
                                obras2.supervisao s
                            INNER JOIN
                                seguranca.usuario u ON u.usucpf = s.usucpf
                            LEFT JOIN
                                obras2.situacaoatividade si ON si.staid = s.staid
                            LEFT JOIN
                                entidade.entidade e ON e.entid = s.supvistoriador
                            LEFT JOIN
                                obras2.realizacaosupervisao rs ON rs.rsuid = s.rsuid
                            LEFT JOIN
                                workflow.documento doc ON doc.docid = s.docid
                            LEFT JOIN
                                obras2.cronograma cro ON cro.croid = s.croid
                            WHERE
                                s.obrid = '" . $obrid . "' AND
                                " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                                s.supstatus = 'A'
                            ORDER BY
                                s.supdata, s.supdtinclusao ASC";
        
        //dbg(simec_htmlentities($sql)); die;

        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }

    public function listaDadosGestorUnidade($obrid, Array $param = array()) {
        $arWhere = array();

        if ($param['not(emsid)']) {
            $arWhere[] = "s.emsid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "s.smiid IS NULL";
        }

        $sql = "SELECT distinct
                    to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                    to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,
                    COALESCE('0', '-') AS aditivo,
                    u.usunome,
                    si.stadesc,
                    rs.rsudsc as responsavel,
                    UPPER(e.entnome) as vistoriador,
                    s.supid,
                    s.usucpf,
                    e.entnumcpfcnpj as cpfvistoriador,
                    s.supdtinclusao,
                    s.croid,
                    cro.crostatus,
                    ( SELECT
                            CASE 
                                WHEN SUM(icovlritem) > 0 THEN 
                                    ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                                ELSE 
                                    0	
                            END AS total
                                    -- SUM(spivlritemsobreobraexecanterior) AS total
                        FROM
                            obras2.cronograma cro
                            JOIN obras2.itenscomposicaoobra i ON cro.croid = i.croid
                        LEFT JOIN
                            obras2.supervisaoitem sic ON sic.icoid = i.icoid
                            AND sic.supid = s.supid
                            AND sic.icoid IS NOT NULL
                            AND sic.ditid IS NULL
                            JOIN obras2.supervisao su ON su.supid = sic.supid
                        WHERE
                            i.icostatus = 'A' AND
                            i.relativoedificacao = 'D' AND
                            i.obrid = '" . $obrid . "' AND
                            i.croid = s.croid AND
                            cro.crostatus IN ('A', 'H') AND su.croid = cro.croid
                    ) as percentual,
                    doc.esdid,
                    s.supdata,
                    s.supid_1,
                    rs.rsuid as realizacaosupervisao_rsuid,
                    CASE 
                    	WHEN validadaPeloSupervisorUnidade = 'N' OR validadaPeloSupervisorUnidade IS NULL THEN '<font style=\"color:red\">Não</font>'
                    	WHEN validadaPeloSupervisorUnidade = 'S' THEN '<font style=\"color:green\">Sim</font>'
                    END as validado,
                    validadaPeloSupervisorUnidade
                FROM
                    obras2.supervisao s
                INNER JOIN
                    seguranca.usuario u ON u.usucpf = s.usucpf
                LEFT JOIN
                    obras2.situacaoatividade si ON si.staid = s.staid
                LEFT JOIN
                    entidade.entidade e ON e.entid = s.supvistoriador
                LEFT JOIN
                    obras2.realizacaosupervisao rs ON rs.rsuid = s.rsuid
                LEFT JOIN
                    workflow.documento doc ON doc.docid = s.docid

                LEFT JOIN
                    obras2.cronograma cro ON s.croid = cro.croid
                WHERE
                    s.obrid = '" . $obrid . "' AND
                    " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                    s.supstatus = 'A'
                ORDER BY
                    s.supdata, s.supdtinclusao ASC";
        
        //dbg(simec_htmlentities($sql), d);

        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }

    public function verificaValidacaoSupervisorUnidae($supid) {
        $sql = "
    		select validadaPeloSupervisorUnidade
			from obras2.supervisao
			where supid = '{$supid}'
    	";
        return $this->pegaUm($sql);
    }

    public static function validaSupervisao($supid) {
        global $db;
        
//        if($supid == '' || $supid == NULL || !isset($supid)){
//            $supid = $this->supid;
//        }
        
        $sql = "
    		update obras2.supervisao
			set validadaPeloSupervisorUnidade = 'S'
			where supid = '{$supid}'
    	";
        $db->executar($sql);
        return $db->commit();
    }

    public function getSupervisao($obrid, $supid, $param = array()) {
        $arWhere = array();

        if (!$param['not(emsid)']) {
            $arWhere[] = "s.emsid IS NULL";
        }

        if (!$param['not(smiid)']) {
            $arWhere[] = "s.smiid IS NULL";
        }

        $sql = "SELECT distinct
                                to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                                to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,
                                COALESCE('0', '-') AS aditivo,
                                u.usunome,
                                si.stadesc,
                                rs.rsudsc as responsavel,
                                UPPER(e.entnome) as vistoriador,
                                s.supid,
                                s.usucpf,
                                e.entnumcpfcnpj as cpfvistoriador,
                                s.supdtinclusao,
                                ( SELECT
                                        CASE 
                                            WHEN SUM(icovlritem) > 0 THEN 
                                                ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                                            ELSE 
                                                0	
                                        END AS total
                                                -- SUM(spivlritemsobreobraexecanterior) AS total
                                    FROM
                                        obras2.itenscomposicaoobra i
                                    LEFT JOIN
                                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                                        AND sic.supid = s.supid
                                        AND sic.icoid IS NOT NULL
                                        AND sic.ditid IS NULL
                                    JOIN obras2.supervisao su ON sic.supid = su.supid
                                    JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A', 'H') AND cro.croid = su.croid
                                    WHERE
                                        i.icostatus = 'A' AND
                                        i.obrid = '" . $obrid . "'
                                ) as percentual,
                                doc.esdid,
                                s.supdata,
                                s.supid_1,
                                rs.rsuid as realizacaosupervisao_rsuid,
                                CASE WHEN s.supprojespecificacoes = 't' THEN 'Sim' ELSE 'Não' END AS projetoespecificacoes,
                                CASE WHEN s.supplacaobra          = 't' THEN 'Sim' ELSE 'Não' END AS placaobra,
                                CASE WHEN s.supdiarioobra         = 't' THEN 'Sim' ELSE 'Não' END AS diarioobra,
                                CASE WHEN s.supplacalocalterreno  = 't' THEN 'Sim' ELSE 'Não' END AS placalocalterreno,
                                CASE WHEN s.supvalidadealvara     = 't' THEN 'Sim' ELSE 'Não' END AS validadealvara,
                                oq.qlbdesc AS qualidadeobra,
                                od.dcndesc AS desempenho,
                                UPPER(s.supobs) AS supobs,
                                UPPER(s.supjusticativaperc) AS supjusticativaperc
                            FROM
                                obras2.supervisao s
                            INNER JOIN
                                seguranca.usuario u ON (u.usucpf = s.usucpf)
                            LEFT JOIN
                                obras2.situacaoatividade si ON (si.staid = s.staid)
                            LEFT JOIN
                                entidade.entidade e ON (e.entid = s.supvistoriador)
                            LEFT JOIN
                                obras2.realizacaosupervisao rs ON (rs.rsuid = s.rsuid)
                            LEFT JOIN
                                workflow.documento doc ON (doc.docid = s.docid)
                            LEFT JOIN 
                                obras2.qualidadeobra oq ON (oq.qlbid = s.qlbid)
                            LEFT JOIN 
                                obras2.desempenhoconstrutora od ON (od.dcnid = s.dcnid)
                            WHERE
                                s.obrid = '" . $obrid . "' AND
                                " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                                s.supstatus = 'A' AND
                                s.supid IN(" . $supid . ")
                            ORDER BY
                                s.supdata, s.supdtinclusao ASC";

        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }

    public function listaFormExtratoSql($obrid) {
        $sql = "SELECT
                            '<div align=\"center\" >
                                    <input type=\"checkbox\" name=\"exibe_' || s.supid || '\" onclick=\"exibirFotos(this, ' || s.supid || ')\" />
                             </div>' AS exibicao,
                            s.supid,
                            to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                            to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,						
                            u.usunome,
                            si.stadesc,
                            s.suprealizacao as responsavel
                    FROM
                            obras2.supervisao s
                    INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf
                    LEFT JOIN obras2.situacaoatividade si ON si.staid = s.staid
                    WHERE
                            s.obrid = {$obrid} AND
                            s.supstatus = 'A'
                    ORDER BY 
                            s.supdtinclusao ASC";

        return $sql;
    }
    
    public function pegaSupidByObraAndEmsid($obrid, $emsid) {
        if (empty($obrid) || empty($emsid)) {
            return '';
        }

        $sql = "SELECT
                            supid
                    FROM
                            obras2.supervisao s 
                    WHERE
                            s.supstatus = 'A' AND
                            s.obrid = {$obrid} AND
                            s.emsid = {$emsid}";

        return $this->pegaUm($sql);
    }

    public function pegaSupidByObraAndSueid($obrid, $sueid) {
        if (empty($obrid) || empty($sueid)) {
            return '';
        }

        $sql = "SELECT
                            supid
                    FROM
                            obras2.supervisao s 
                    WHERE
                            s.supstatus IN ('A', 'P') AND
                            s.obrid = {$obrid} AND
                            s.sueid = {$sueid}";

//        ver($sql,d);
                            
        return $this->pegaUm($sql);
    }

    public function pegaSupidByObraAndSmiid($obrid, $smiid) {
        if (empty($obrid) || empty($smiid)) {
            return '';
        }

        $sql = "SELECT
                            supid
                    FROM
                            obras2.supervisao s 
                    WHERE
                            s.supstatus = 'A' AND
                            s.obrid = {$obrid} AND
                            s.smiid = {$smiid}";

        return $this->pegaUm($sql);
    }

    public function pegaSupidByObraAndSfndeid($obrid, $sfndeid) {
        if (empty($obrid) || empty($sfndeid)) {
            return '';
        }

        $sql = "SELECT
                            supid
                    FROM
                            obras2.supervisao s 
                    WHERE
                            s.supstatus = 'A' AND
                            s.obrid = {$obrid} AND
                            s.sfndeid = {$sfndeid}";
        // ver($sql,d);
        return $this->pegaUm($sql);
    }

    public function pegaUltSupidByObra($obrid, Array $param = array()) {
        $arJoin = array();
        $arWhere = array();

        if ($param['not(emsid)']) {
            $arWhere[] = "s.emsid IS NULL";
        }

        if ($param['not(sueid)']) {
            $arWhere[] = "s.sueid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "s.smiid IS NULL";
        }

        if ($param['is(smiid)']) {
            $arWhere[] = "s.smiid IS NOT NULL";
        }

        if ($param['is(sfndeid)']) {
            $arWhere[] = "s.sfndeid IS NOT NULL";
        }

        if ($param['validadoSupervisor']) {
            $arWhere[] = "validadaPeloSupervisorUnidade = 'S'";
        }

        if ($param['empid'] && $param['is(sueid)']) {
            $arJoin[] = "JOIN obras2.supervisaoempresa se ON se.sueid = s.sueid";
            $arWhere[] = "se.empid = {$param['empid']}";
        } elseif ($param['empid'] && $param['is(smiid)']) {
            $arJoin[] = "JOIN obras2.supervisaomi sm ON sm.smiid = s.smiid";
            $arWhere[] = "sm.empid = {$param['empid']}";
        } elseif ($param['empid']) {
            $arJoin[] = "JOIN obras2.empreendimentosupervisao es ON es.emsid = s.emsid";
            $arWhere[] = "es.empid = {$param['empid']}";
        }
        
        $sql = "SELECT
                    supid
            FROM
                    obras2.supervisao s
            " . (count($arJoin) ? implode(" ", $arJoin) : "") . "	
            WHERE
                    s.supstatus = 'A' AND
                    " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                s.obrid = {$obrid}
            ORDER BY
                    s.supdata DESC, s.supid DESC
            LIMIT 1";
        
        //dbg(simec_htmlentities($sql), d);
        
        return $this->pegaUm($sql);
    }

    public function pegaSituacaoVistoria($obrid, Array $param = array()) {
        if (empty($obrid)) {
            return '';
        }

        $arWhere = array();
        if ($param['not(emsid)']) {
            $arWhere[] = "emsid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "smiid IS NULL";
        }

        $sql = "SELECT
                    staid
            FROM
                    obras2.supervisao
            WHERE
                    supstatus = 'A' AND
                supid IN (SELECT 
                                MAX(supid)
                          FROM
                                obras2.supervisao
                          WHERE
                                supstatus = 'A' AND
                                " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . " 
                                obrid = {$obrid});";

        return $this->pegaUm($sql);
    }

    public function antesSalvar() {

        if ($this->emsid) {
            return $this->antesSalvarVistoriaObra();
        } elseif ($this->sueid) {
            return $this->antesSalvarVistoriaEmpresa();
        } elseif ($this->smiid) {
            return $this->antesSalvarVistoriaMI();
        } elseif ($this->sfndeid) {
            return $this->antesSalvarVistoriaFNDE();
        } else {
            return $this->antesSalvarVistoriaObjeto();
        }
    }

    public function antesSalvarVistoriaObjeto() {
        $dados = $_POST;
        // Observação limite 500 caracteres
        // Somente pode selecionar situação concluída, se estiver 100% e perfis permitidos

        $this->createVistoria = ( empty($dados['supid']) ? true : false );

        $cronograma = new Cronograma();

        $croid = ($_REQUEST['croid']) ? $_REQUEST['croid'] : $cronograma->getIdCronogramaObra($_SESSION['obras2']['obrid']);
        $croid = (!empty($dados['croid'])) ? $dados['croid']  : $croid;

        $this->supid = $dados['supid'];
        $this->croid = $croid;
        $this->obrid = $_SESSION['obras2']['obrid'];
        $this->usucpf = $_SESSION['usucpf'];
        $this->supobs = ( ($dados["supobs"] > 5000) ? substr($dados["supobs"], 0, 5000) : $dados["supobs"] );
        $this->supjusticativaperc = ( ($dados["supjusticativaperc"] > 5000) ? substr($dados["supjusticativaperc"], 0, 5000) : $dados["supjusticativaperc"] );
        $this->staid = $dados['staid'];

        $this->supdata = formata_data_sql($dados['supdata']);
        $this->supdatafimobra = formata_data_sql($dados['supdatafimobra']);
        $this->supparecerorgao = $dados['supparecerorgao'];
        $this->supstatus = 'A';
        $this->supvistoriador = !empty($dados['entidvistoriador']) ? $dados['entidvistoriador'] : NULL;
        $this->supprojespecificacoes = ( $dados['supprojespecificacoes'] == 1 ? 't' : 'f' );
        $this->supplacaobra = ( $dados['supplacaobra'] == 1 ? 't' : 'f' );
        $this->supplacalocalterreno = ( $dados['supplacalocalterreno'] == 1 ? 't' : 'f' );
        $this->supvalidadealvara = ( $dados['supvalidadealvara'] == 1 ? 't' : 'f' );
        $this->supdiarioobra = ( $dados['supdiarioobra'] == 1 ? 't' : 'f' );
        $this->qlbid = !empty($dados['qlbid']) ? $dados['qlbid'] : NULL;
        $this->dcnid = !empty($dados['dcnid']) ? $dados['dcnid'] : NULL;
        $this->stpid = !empty($dados['stpid']) ? $dados['stpid'] : NULL;

        if ($dados['staid'] == STAID_EXECUCAO && $_FILES["arqidosexecucao"]['name']) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec(null, null, "obras2");
            $file->setPasta('obras2');
            $file->setUpload(null, 'arqidosexecucao', false);
            $this->arqidosexecucao = $file->getIdArquivo();
        }

//        if( possuiPerfil(PERFIL_SUPERVISORUNIDADE) ){
        $this->suprealizacao = 'Instituição';
        $this->rsuid = '1';
//        }elseif( possuiPerfil(PERFIL_EMPRESA) ){
//            $this->suprealizacao = 'Empresa';
//            $this->rsuid         = '3';
//        }else{
//            $this->suprealizacao = 'MEC';
//            $this->rsuid         = '2';
//        }

        $arqid_planilha = null;
        $arquivo = $_FILES["arquivo_planilha"];
        if ($dados['staid'] == STAID_CONCLUIDO && $_FILES["arquivo_planilha"] && $arquivo['error'] == 0 && $arquivo["name"]) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec(null, null, "obras2");
            $file->setPasta('obras2');
            $returnUpload = $file->setUpload(null, 'arquivo_planilha', false);

            if ($returnUpload) {
                $arqid_planilha = $file->getIdArquivo();
            } else {
                $this->rollback();
                $urlSupid = "&supid={$_SESSION['obras2']['supid']}";

                enviaEmailChiavicatti($arquivo);

                die('<script type="text/javascript">
                            alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
                            window.location.href = \'?modulo=principal/inserir_vistoria&acao=A' . $urlSupid . '\'; 
                     </script>');
            }
        } elseif ($arquivo['error'] != 0 && $arquivo['error'] != 4) {
            if ($arquivo['error'] == 3) {
                echo('<script type="text/javascript">
                            alert(\'Falha no upload do arquivo: ' . $arquivo['name'] . '\nAnexe-o novamente!\');
                      </script>');
            } else {
                $this->rollback();
                $urlSupid = "&supid={$_SESSION['obras2']['supid']}";

                enviaEmailChiavicatti($arquivo);

                die('<script type="text/javascript">
                            alert(\'Devido a falha no upload de arquivo os dados não foram salvos.\nFavor repetir a operação.\');
                            window.location.href = \'?modulo=principal/inserir_vistoria&acao=A' . $urlSupid . '\'; 
                     </script>');
            }
        }

        $this->arqidplanilha = $arqid_planilha;

        $arqid_termo = null;
        $arquivo = $_FILES["arquivo_termo"];
        if ($dados['staid'] == STAID_CONCLUIDO && $_FILES["arquivo_termo"] && $arquivo["name"] && $arquivo["error"] == 0) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec(null, null, "obras2");
            $file->setPasta('obras2');
            $file->setUpload(null, 'arquivo_termo', false);
            $arqid_termo = $file->getIdArquivo();
        }
        $this->arqidtermo = $arqid_termo;

        return true;
    }

    public function antesSalvarVistoriaObra() {
        return true;
    }

    public function antesSalvarVistoriaEmpresa() {
        return true;
    }

    public function antesSalvarVistoriaMI() {
        return true;
    }

    public function antesSalvarVistoriaFNDE() {
        return true;
    }

    public function depoisSalvar() {

        if ($this->emsid) {
            return $this->depoisSalvarVistoriaObra();
        } elseif ($this->sueid) {
            return $this->depoisSalvarVistoriaEmpresa();
        } elseif ($this->smiid) {
            return $this->depoisSalvarVistoriaMI();
        } elseif ($this->sfndeid) {
            return $this->depoisSalvarVistoriaFNDE();
        } else {
            return $this->depoisSalvarVistoriaObjeto();
        }
    }

    public function depoisSalvarVistoriaObjeto() {
        $dados = $_POST;

        $supervisaoItem = new SupervisaoItem();
//        $estrutura     = new EstruturaObra();
//        // Salva estruturas
//        if ( is_array($dados['edf_spiid']) ){
//            for ($i=0; $i < count($dados['edf_spiid']); $i++){
//                if ( empty($dados['edf_spiid'][$i]) == false )
//                    $supervisaoItem->carregarPorId( $dados['edf_spiid'][$i] );
//
//                $supervisaoItem->spiid                           = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
//                $supervisaoItem->supid                          = $this->supid;
//                $supervisaoItem->eobid                           = $dados['eobid'][$i];
////                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlritemexecanterior'][$i] );
////                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_supvlritemsobreobraexecanterior'][$i] );
//                $supervisaoItem->supvlrfinanceiroanterior        = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_suppercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['edf_suppercsobreobrainfsupervisor'][$i] );
//                $supervisaoItem->supvlrinfsupervisor             = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlrfinanceiroinfsupervisor   = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->salvar();
//                $supervisaoItem->clearDados();
//
//                $estrutura->carregarPorId( $dados['eobid'][$i] );
//                $estrutura->eobpercexecutado = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i] );
//                $estrutura->salvar();
//
//            }
//        }

        $percentExec = 0;
        // Salva etapas
        $itemComposicao = new ItensComposicaoObras();
        if (is_array($dados['ico_spiid'])) {
            for ($i = 0; $i < count($dados['ico_spiid']); $i++) {
                if (empty($dados['ico_spiid'][$i]) == false) {
                    $supervisaoItem->carregarPorId($dados['ico_spiid'][$i]);
                }

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->icoid = $dados['icoid'][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['ico_supvlritemexecanterior'][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['ico_supvlritemsobreobraexecanterior'][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['ico_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['ico_spipercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['ico_spipercsobreobrainfsupervisor'][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['ico_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spitotitemsupervisor = MoedaToBd($dados['ico_spitotitemsupervisor'][$i] == '' ? 0 : $dados['ico_spitotitemsupervisor'][$i]);

                $supervisaoItem->spivlrfinanceiroanterior = (is_numeric($supervisaoItem->spivlrfinanceiroanterior) ? $supervisaoItem->spivlrfinanceiroanterior : 0);
                $supervisaoItem->spivlritemexecanterior = (is_numeric($supervisaoItem->spivlritemexecanterior) ? $supervisaoItem->spivlritemexecanterior : 0);
                $supervisaoItem->spivlritemsobreobraexecanterior = (is_numeric($supervisaoItem->spivlritemsobreobraexecanterior) ? $supervisaoItem->spivlritemsobreobraexecanterior : 0);
                $supervisaoItem->spivlrinfsupervisor = (is_numeric($supervisaoItem->spivlrinfsupervisor) ? $supervisaoItem->spivlrinfsupervisor : 0);
                $supervisaoItem->spivlrfinanceiroinfsupervisor = (is_numeric($supervisaoItem->spivlrfinanceiroinfsupervisor) ? $supervisaoItem->spivlrfinanceiroinfsupervisor : 0);

                $itemComposicao->carregarPorId($dados['icoid'][$i]);
                if($itemComposicao->relativoedificacao == 'D')
                $percentExec += $supervisaoItem->spivlritemsobreobraexecanterior;

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $itemComposicao->icopercexecutado = MoedaToBd($dados['ico_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$i]);
                $itemComposicao->salvar();
                $itemComposicao->clearDados();
            }
        }

        $this->percentExec = $percentExec;

        // Salva detalhamento
        $detalheitem = new DetalheItem();
        if (is_array($dados['dit_spiid'])) {
            for ($i = 0; $i < count($dados['dit_spiid']); $i++) {
                if (empty($dados['dit_spiid'][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dit_spiid'][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid'][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dit_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dit_spipercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['dit_spipercsobreobrainfsupervisor'][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dit_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dit_spivlrmetafisicasupervisor'][$dados['ditid'][$i]][0] ? MoedaToBd($dados['dit_spivlrmetafisicasupervisor'][$dados['ditid'][$i]][0]) : NULL);

                $supervisaoItem->spivlrfinanceiroanterior = (is_numeric($supervisaoItem->spivlrfinanceiroanterior) ? $supervisaoItem->spivlrfinanceiroanterior : 0);
                $supervisaoItem->spivlritemexecanterior = (is_numeric($supervisaoItem->spivlritemexecanterior) ? $supervisaoItem->spivlritemexecanterior : 0);
                $supervisaoItem->spivlritemsobreobraexecanterior = (is_numeric($supervisaoItem->spivlritemsobreobraexecanterior) ? $supervisaoItem->spivlritemsobreobraexecanterior : 0);
                $supervisaoItem->spivlrinfsupervisor = (is_numeric($supervisaoItem->spivlrinfsupervisor) ? $supervisaoItem->spivlrinfsupervisor : 0);
                $supervisaoItem->spivlrfinanceiroinfsupervisor = (is_numeric($supervisaoItem->spivlrfinanceiroinfsupervisor) ? $supervisaoItem->spivlrfinanceiroinfsupervisor : 0);
                $supervisaoItem->spivlrmetafisicasupervisor = (is_numeric($supervisaoItem->spivlrmetafisicasupervisor) ? $supervisaoItem->spivlrmetafisicasupervisor : 0);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid'][$i]);
//                $detalheitem->ditdsc = addslashes($detalheitem->ditdsc); -- Pau no addslashes
                $detalheitem->ditdsc = $detalheitem->ditdsc;
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dit_spivlrinfsupervisor'][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        // Salva detalhamentos Filhos
        $detalheitem = new DetalheItem();
        if (is_array($dados['dtf_spiid'])) {
            for ($i = 0; $i < count($dados['dtf_spiid']); $i++) {
                if (empty($dados['dtf_spiid'][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dtf_spiid'][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid_filho'][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dtf_spipercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['dtf_spipercsobreobrainfsupervisor'][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dtf_spivlrmetafisicasupervisor'][$dados['ditid_filho'][$i]][0] ? MoedaToBd($dados['dtf_spivlrmetafisicasupervisor'][$dados['ditid_filho'][$i]][0]) : NULL);

                $supervisaoItem->spivlrfinanceiroanterior = (is_numeric($supervisaoItem->spivlrfinanceiroanterior) ? $supervisaoItem->spivlrfinanceiroanterior : 0);
                $supervisaoItem->spivlritemexecanterior = (is_numeric($supervisaoItem->spivlritemexecanterior) ? $supervisaoItem->spivlritemexecanterior : 0);
                $supervisaoItem->spivlritemsobreobraexecanterior = (is_numeric($supervisaoItem->spivlritemsobreobraexecanterior) ? $supervisaoItem->spivlritemsobreobraexecanterior : 0);
                $supervisaoItem->spivlrinfsupervisor = (is_numeric($supervisaoItem->spivlrinfsupervisor) ? $supervisaoItem->spivlrinfsupervisor : 0);
                $supervisaoItem->spivlrfinanceiroinfsupervisor = (is_numeric($supervisaoItem->spivlrfinanceiroinfsupervisor) ? $supervisaoItem->spivlrfinanceiroinfsupervisor : 0);
                $supervisaoItem->spivlrmetafisicasupervisor = (is_numeric($supervisaoItem->spivlrmetafisicasupervisor) ? $supervisaoItem->spivlrmetafisicasupervisor : 0);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid_filho'][$i]);
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        $dados['staid'] = !empty($dados['staid']) ? $dados['staid'] : projeto_pega_situacao_vistoria($_SESSION['obras2']['obrid']);

        if (!empty($dados['staid'])) {
//            if($this->rsuid == '3'){
//                $atualizaSituacaoParaEmpresa = ", stoidsupemp = {$dados["staid"]}";
//            }
            $atualizaSituacaoParaEmpresa = "";
//            $sql = "UPDATE
//                        pde.atividade
//                    SET
//                        staid = {$dados["staid"]},
//                        atidtvistoria = now()
//                        {$atualizaSituacaoParaEmpresa}
//                    WHERE
//                        obrid = {$_SESSION['obras2']['obrid']}";

            $obra = new Obras($_SESSION['obras2']['obrid']);
            $obra->staid = $dados['staid'];
            $obra->obrdtvistoria = date('Y-m-d');
            $obra->salvar();
        }

//             // se for elaboração de projetos
//         if ($dados['staid'] == '4'){
//            $sql = "UPDATE
//                        obras.obrainfraestrutura
//                    SET
//                        obrlincambiental  = '{$dados['obrlincambiental']}',
//                        obraprovpatrhist  = '{$dados['obraprovpatrhist']}',
//                        obrdtprevprojetos = '".formata_data_sql($dados["obrdtprevprojetos"])."'
//                    WHERE
//                        obrid = {$_SESSION['obras2']["obrid"]}";
//
//            $this->executar($sql);
//
//         }else

        $histParalisacao = new HistoricoParalisacao();
        // se a situação for paralizada, cria o histórico
        if ($dados['staid'] == STAID_PARALISADO) {

            $arqid = null;
            $arqidboletim = null;
            $arquivo = $_FILES["arquivo"];
            $arquivoOS = $_FILES["arqidosparalisacao"];
            $documentoEmbargo = $_FILES["documentoembargo"];

            if ($dados['tplid'] == TPLID_EMBARGO && $_FILES["documentoembargo"] && $documentoEmbargo["name"]) {



                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'documentoembargo', false);
                $arqiddocumentoembargo = $file->getIdArquivo();
                //ver($arqiddocumentoembargo);

            }

            if ($dados['tplid'] == TPLID_OS_PARALISACAO && $_FILES["arqidosparalisacao"] && $arquivoOS["name"]) {
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'arqidosparalisacao', false);
                $arqidosparalisacao = $file->getIdArquivo();
            }

            if ($dados['tplid'] == TPLID_CONTRATO_RESCINDIDO && $_FILES["arquivo"] && $arquivo["name"]) {
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'arquivo', false);
                $arqid = $file->getIdArquivo();
            }

            if ($dados['tplid'] == TPLID_CONTRATO_RESCINDIDO && $_FILES["arquivoboletim"] && $arquivo["name"] && $arquivo["error"] == 0) {
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                $file = new FilesSimec(null, null, "obras2");
                $file->setPasta('obras2');
                $file->setUpload(null, 'arquivoboletim', false);
                $arqidboletim = $file->getIdArquivo();
            }
//		    $arDado['arqid'] = ($arqid ? $arqid : $arDado['arqid']);

            if ($this->createVistoria) {

                

                $arDados = array('tplid' => $dados['tplid'],
                    'obrid' => $_SESSION['obras2']['obrid'],
                    'supidparalisacao' => $this->supid,
                    'arqid' => $arqid,
                    'arqidboletim' => $arqidboletim,
                    'arqidosparalisacao' => $arqidosparalisacao,
                    'arqiddocumentoembargo' => $arqiddocumentoembargo,
                    'hprobs' => $dados['hprobs1'],
                    'hprdtstatus' => 'A',
                    'stpid' => $dados['stpid'],
                    'hprdtinclusao' => date('Y/m/d'));
                $histParalisacao->popularDadosObjeto($arDados)
                        ->salvar();
                $histParalisacao->clearDados();

//                $sql = "INSERT INTO
//                            obras2.historicoparalisacao (tplid, obrid, supidparalisacao, hprobs,
//                                                        hprdtstatus, hprdtinclusao)
//                        VALUES
//                            ({$dados['tplid']}, {$_SESSION['obras2']['obrid']}, {$this->supid},
//                             '{$dados['hprobs1']}', 'A', now())";
//
//                $this->executar($sql);
            } else {
                $hprid = $histParalisacao->pegaHpridParalisado(array('obrid' => $_SESSION['obras2']['obrid']));

//                $sql = "SELECT
//                            hprid
//                        FROM
//                            obras2.historicoparalisacao
//                        WHERE
//                            obrid = {$_SESSION['obras2']['obrid']} AND
//                            supidparalisacao IS NOT NULL AND
//                            supidliberacao IS NULL";
//
//                $hprid = $this->pegaUm($sql);

                if (empty($hprid)) {
                    $arDados = array('tplid'            => $dados['tplid'],
                                     'obrid'            => $_SESSION['obras2']['obrid'],
                                     'supidparalisacao' => $this->supid,
                                     'arqid'            => (empty($arqid)) ? null : $arqid,
                                     'arqidboletim'     => (empty($arqidboletim)) ? null : $arqidboletim,
                                     'arqidosparalisacao'=> (empty($arqidosparalisacao)) ? null : $arqidosparalisacao,
                                     'arqiddocumentoembargo'=> (empty($arqiddocumentoembargo)) ? null : $arqiddocumentoembargo,
                                     'hprobs'           => $dados['hprobs1'],
                                     'stpid' => $dados['stpid'],
                                     'hprdtstatus'      => 'A',
                                     'hprdtinclusao'    => date('Y/m/d'));
                    
                    if ( empty($arDado['arqid']) ){
                        $arCamposNulo[] = 'arqid';
                    }
                    if ( empty($arDado['arqidboletim']) ){
                        $arCamposNulo[] = 'arqidboletim';
                    }
                    if ( empty($arDado['arqidosparalisacao']) ){
                        $arCamposNulo[] = 'arqidosparalisacao';
                    }
                    if ( empty($arDado['arqiddocumentoembargo']) ){
                        $arCamposNulo[] = 'arqiddocumentoembargo';
                    }


                    $histParalisacao->popularDadosObjeto($arDados)
                            ->salvar();
                    $histParalisacao->clearDados();

//                    $sql = "INSERT INTO
//                                obras2.historicoparalisacao (tplid, obrid, supidparalisacao, hprobs,
//                                                            hprdtstatus, hprdtinclusao)
//                            VALUES
//                                ({$dados['tplid']}, {$_SESSION['obras2']['obrid']}, {$this->supid},
//                                 '{$dados['hprobs1']}', 'A', now())";
//
//                    $this->executar($sql);
                }

                $histParalisacao->carregaPorSupidParalisacao($this->supid);
                $hprdtliberacao = $histParalisacao->hprdtliberacao;
                $histParalisacao->arqid = ( $arqid ? $arqid : $histParalisacao->arqid );
                $histParalisacao->arqidboletim = ( $arqidboletim ? $arqidboletim : $histParalisacao->arqidboletim );
                $histParalisacao->arqidosparalisacao = ( $arqidosparalisacao ? $arqidosparalisacao : $histParalisacao->arqidosparalisacao );

//                $hprdtliberacao = $this->pegaUm("SELECT
//                                                    hprdtliberacao
//                                                 FROM
//                                                    obras2.historicoparalisacao
//                                                 WHERE
//                                                    supidparalisacao = {$this->supid}");

                $arCamposNulo = array();
                if (empty($dados['tplid'])) {
                    $dados['tplid'] = null;
                    $arCamposNulo[] = 'tplid';
                } else {
                    $histParalisacao->tplid = $dados['tplid'];
                }

                $histParalisacao->hprobs = $dados['hprobs1'];

                if (!empty($hprdtliberacao)) {
                    $histParalisacao->supidliberacao = $this->supid;
                } else {
                    $histParalisacao->supidparalisacao = $this->supid;
                }
                
                if(empty($histParalisacao->arqid)){
                    $histParalisacao->arqid = null;
                    $arCamposNulo[]         = 'arqid';
                }
                
                if(empty($histParalisacao->arqidboletim)){
                    $histParalisacao->arqidboletim = null;
                    $arCamposNulo[]                = 'arqidboletim';
                }

                $v =  $histParalisacao->arqidosparalisacao;
                if(empty($v)){
                    $histParalisacao->arqidosparalisacao = null;
                    $arCamposNulo[]                = 'arqidosparalisacao';
                }

                $histParalisacao->salvar(true, true, $arCamposNulo);

//                $tplid = $dados['tplid'] ? $dados['tplid'] : 'null';
//
//                $sql = "UPDATE
//                            obras2.historicoparalisacao
//                        SET
//                            tplid = {$tplid},
//                            hprobs = '{$dados['hprobs1']}',
//                            hprdtstatus = 'A'
//                        WHERE
//                            ". (!empty($hprdtliberacao) ? 'supidliberacao' : 'supidparalisacao') . " = {$this->supid}";
//
//                $this->executar($sql);
            }

//			if ( $dados['tplid'] == TPLID_CONTRATO_RESCINDIDO ){
//	        	$obra = new Obras( $_SESSION['obras2']['obrid'] );
//				wf_alterarEstado($obra->docid, AEDID_OBJ_EXECUCAO_PARALISADO, '', array());
//	        	$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
//				wf_alterarEstado($empreendimento->docid, AEDID_EMP_EXEC_PARALISAR, '', array());
//	        }            

        } else {
            if ($this->createVistoria) {
                $hprid = $histParalisacao->pegaHpridParalisado(array('obrid' => $_SESSION['obras2']['obrid']));

//                $sql = "SELECT
//                            hprid
//                        FROM
//                            obras2.historicoparalisacao
//                        WHERE
//                            obrid = {$_SESSION['obras2']['obrid']} AND
//                            supidparalisacao IS NOT NULL AND
//                            supidliberacao IS NULL AND
//                            hprdtstatus = 'A'";
//
//                $hprid = $this->pegaUm($sql);

                if (!empty($hprid)) {
                    $histParalisacao->carregarPorId($hprid);
                    $arDados = array('supidliberacao' => $this->supid,
                        'hprdtstatus' => 'I',
                        'hprdtliberacao' => date('Y/m/d'));
                    $histParalisacao->popularDadosObjeto($arDados)
                            ->salvar();
                    $histParalisacao->clearDados();

//                    $sql = "UPDATE
//                                obras2.historicoparalisacao
//                            SET
//                                supidliberacao = {$this->supid},
//                                hprdtliberacao = now(),
//                                hprdtstatus = 'I'
//                            WHERE
//                                hprid = {$hprid}";
//
//                    $this->executar($sql);
                }
            } else {
                $histParalisacao->atualizaStatusPorObrid($_SESSION['obras2']['obrid']);

//                $sql = "UPDATE obras2.historicoparalisacao SET hprdtstatus = 'I'
//                        WHERE obrid = {$_SESSION['obras2']['obrid']}";
//
//                $this->executar($sql);
            }
        }

        if ($dados['staid'] == STAID_CONCLUIDO) {
            $obra = new Obras($_SESSION['obras2']['obrid']);
            $esdid = pegaEstadoObra($obra->docid);

            if ($esdid == ESDID_OBJ_EXECUCAO) {
                wf_alterarEstado($obra->docid, AEDID_OBJ_EXECUCAO_CONCLUIDO, '', array('obrid' => $obra->obrid));
            }
        } elseif ($dados['staid'] == STAID_EXECUCAO) {
            $obra = new Obras($_SESSION['obras2']['obrid']);
            $esdid = pegaEstadoObra($obra->docid);

            if ($esdid == ESDID_OBJ_CONCLUIDO) {
                wf_alterarEstado($obra->docid, AEDID_OBJ_CONCLUIDO_EXECUCAO, '', array('obrid' => $obra->obrid));
            } elseif ($esdid == ESDID_OBJ_PARALISADO) {
                wf_alterarEstado($obra->docid, AEDID_OBJ_PARALISADO_EXECUCAO, '', array('obrid' => $obra->obrid));
            }
        } elseif ($dados['staid'] == STAID_PARALISADO) {
            $obra = new Obras($_SESSION['obras2']['obrid']);
            $esdid = pegaEstadoObra($obra->docid);

            if ($esdid == ESDID_OBJ_EXECUCAO) {
                wf_alterarEstado($obra->docid, AEDID_OBJ_EXECUCAO_PARALISADO, '', array('obrid' => $obra->obrid));
            }/* elseif ( $esdid == ESDID_OBJ_CONCLUIDO ){
              wf_alterarEstado($obra->docid, AEDID_OBJ_CONCLUIDO_PARALISADO, '', array('obrid' => $obra->obrid));
              } */
        }

        if ($this->createVistoria) {
            $situacaoAtividade = new SituacaoAtividade($this->staid);


            $regAtividade = new RegistroAtividade();
            $arDado = array();
            $arDado['obrid'] = $this->obrid;
            $arDado['usucpf'] = $_SESSION['usucpf'];
            $arDado['rgaautomatica'] = 't';
            $arDado['rgadscsimplificada'] = 'VISTORIA INSERIDA';
            $arDado['rgadsccompleta'] = "ID acompanhamento: {$this->supid}
Data do acompanhamento: " . formata_data($this->supdata) . "
CPF: {$_SESSION['usucpf']}
Situação: {$situacaoAtividade->stadesc}
% do acompanhamento: {$this->percentExec}
Quando: " . date('d/m/Y');
            $regAtividade->popularDadosObjeto($arDado)
                    ->salvar();
        } else {
            $situacaoAtividade = new SituacaoAtividade($this->staid);

            $regAtividade = new RegistroAtividade();
            $arDado = array();
            $arDado['obrid'] = $this->obrid;
            $arDado['usucpf'] = $_SESSION['usucpf'];
            $arDado['rgaautomatica'] = 't';
            $arDado['rgadscsimplificada'] = 'VISTORIA EDITADA';
            $arDado['rgadsccompleta'] = "ID acompanhamento: {$this->supid}
Data do acompanhamento: " . formata_data($this->supdata) . "
CPF: {$_SESSION['usucpf']}
Situação: {$situacaoAtividade->stadesc}
% do acompanhamento: {$this->percentExec}
Quando: " . date('d/m/Y');
            $regAtividade->popularDadosObjeto($arDado)
                    ->salvar();
        }

        //Atualiza fotos
        atualizarFotosVistoria($this->supid);
        return true;
    }

    public function depoisSalvarVistoriaObra() {
        $dados = $_POST;

        $supervisaoItem = new SupervisaoItem();
        $itemComposicao = new ItensComposicaoObras();
        $detalheitem = new DetalheItem();
//        $estrutura     = new EstruturaObra();
//        // Salva estruturas
//        if ( is_array($dados['edf_spiid']) ){
//            for ($i=0; $i < count($dados['edf_spiid']); $i++){
//                if ( empty($dados['edf_spiid'][$i]) == false )
//                    $supervisaoItem->carregarPorId( $dados['edf_spiid'][$i] );
//
//                $supervisaoItem->spiid                           = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
//                $supervisaoItem->supid                          = $this->supid;
//                $supervisaoItem->eobid                           = $dados['eobid'][$i];
////                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlritemexecanterior'][$i] );
////                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_supvlritemsobreobraexecanterior'][$i] );
//                $supervisaoItem->supvlrfinanceiroanterior        = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_suppercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['edf_suppercsobreobrainfsupervisor'][$i] );
//                $supervisaoItem->supvlrinfsupervisor             = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlrfinanceiroinfsupervisor   = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->salvar();
//                $supervisaoItem->clearDados();
//
//                $estrutura->carregarPorId( $dados['eobid'][$i] );
//                $estrutura->eobpercexecutado = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i] );
//                $estrutura->salvar();
//
//            }
//        }
        $percentExec = 0;
        // Salva etapas
        if (is_array($dados['ico_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['ico_spiid'][$this->obrid]); $i++) {
                if (empty($dados['ico_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['ico_spiid'][$this->obrid][$i]);
                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->icoid = $dados['icoid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['ico_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['ico_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );


                $itemComposicao->carregarPorId($dados['icoid'][$this->obrid][$i]);
                if($itemComposicao->relativoedificacao == 'D')
                    $percentExec += $supervisaoItem->spivlritemsobreobraexecanterior;

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $itemComposicao->icopercexecutado = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i]);
                $itemComposicao->salvar();
                $itemComposicao->clearDados();
            }
        }

        $this->percentExec = $percentExec;

        // Salva detalhamento
        if (is_array($dados['dit_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dit_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dit_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dit_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0] ? MoedaToBd($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0]) : NULL);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid'][$this->obrid][$i]);
//                $detalheitem->ditdsc = addslashes($detalheitem->ditdsc);
                $detalheitem->ditdsc = $detalheitem->ditdsc;
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        // Salva detalhamentos Filhos
        if (is_array($dados['dtf_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dtf_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dtf_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dtf_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid_filho'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0] ? MoedaToBd($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0]) : NULL);
                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid_filho'][$this->obrid][$i]);
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        return true;
    }

    public function depoisSalvarVistoriaMI() {
        $dados = $_POST;

        $supervisaoItem = new SupervisaoItem();
        $itemComposicao = new ItensComposicaoObras();
        $detalheitem = new DetalheItem();
//        $estrutura     = new EstruturaObra();
//        // Salva estruturas
//        if ( is_array($dados['edf_spiid']) ){
//            for ($i=0; $i < count($dados['edf_spiid']); $i++){
//                if ( empty($dados['edf_spiid'][$i]) == false )
//                    $supervisaoItem->carregarPorId( $dados['edf_spiid'][$i] );
//
//                $supervisaoItem->spiid                           = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
//                $supervisaoItem->supid                          = $this->supid;
//                $supervisaoItem->eobid                           = $dados['eobid'][$i];
////                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlritemexecanterior'][$i] );
////                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_supvlritemsobreobraexecanterior'][$i] );
//                $supervisaoItem->supvlrfinanceiroanterior        = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_suppercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['edf_suppercsobreobrainfsupervisor'][$i] );
//                $supervisaoItem->supvlrinfsupervisor             = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlrfinanceiroinfsupervisor   = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->salvar();
//                $supervisaoItem->clearDados();
//
//                $estrutura->carregarPorId( $dados['eobid'][$i] );
//                $estrutura->eobpercexecutado = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i] );
//                $estrutura->salvar();
//
//            }
//        }
        $percentExec = 0;
        // Salva etapas
        if (is_array($dados['ico_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['ico_spiid'][$this->obrid]); $i++) {
                if (empty($dados['ico_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['ico_spiid'][$this->obrid][$i]);

                $itemComposicao->carregarPorId($dados['icoid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->icoid = $dados['icoid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['ico_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['ico_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spitotitemsupervisor            = MoedaToBd($dados['ico_spitotitemsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spitotitemsupervisor'][$this->obrid][$i]);

                if($itemComposicao->relativoedificacao == 'D')
                    $percentExec += $supervisaoItem->spivlritemsobreobraexecanterior;

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $itemComposicao->icopercexecutado = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i]);
                $itemComposicao->salvar();
                $itemComposicao->clearDados();
            }
        }

        $this->percentExec = $percentExec;

        // Salva detalhamento
        if (is_array($dados['dit_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dit_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dit_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dit_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0] ? MoedaToBd($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0]) : NULL);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid'][$this->obrid][$i]);
//                $detalheitem->ditdsc = addslashes($detalheitem->ditdsc);
                $detalheitem->ditdsc = $detalheitem->ditdsc;
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        // Salva detalhamentos Filhos
        if (is_array($dados['dtf_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dtf_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dtf_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dtf_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid_filho'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0] ? MoedaToBd($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0]) : NULL);
                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid_filho'][$this->obrid][$i]);
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        return true;
    }

    public function depoisSalvarVistoriaFNDE() {
        $dados = $_POST;

        $supervisaoItem = new SupervisaoItem();
        $itemComposicao = new ItensComposicaoObras();
        $detalheitem = new DetalheItem();
//        $estrutura     = new EstruturaObra();
//        // Salva estruturas
//        if ( is_array($dados['edf_spiid']) ){
//            for ($i=0; $i < count($dados['edf_spiid']); $i++){
//                if ( empty($dados['edf_spiid'][$i]) == false )
//                    $supervisaoItem->carregarPorId( $dados['edf_spiid'][$i] );
//
//                $supervisaoItem->spiid                           = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
//                $supervisaoItem->supid                          = $this->supid;
//                $supervisaoItem->eobid                           = $dados['eobid'][$i];
////                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlritemexecanterior'][$i] );
////                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_supvlritemsobreobraexecanterior'][$i] );
//                $supervisaoItem->supvlrfinanceiroanterior        = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_suppercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['edf_suppercsobreobrainfsupervisor'][$i] );
//                $supervisaoItem->supvlrinfsupervisor             = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlrfinanceiroinfsupervisor   = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->salvar();
//                $supervisaoItem->clearDados();
//
//                $estrutura->carregarPorId( $dados['eobid'][$i] );
//                $estrutura->eobpercexecutado = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i] );
//                $estrutura->salvar();
//
//            }
//        }
        $percentExec = 0;
        // Salva etapas
        if (is_array($dados['ico_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['ico_spiid'][$this->obrid]); $i++) {
                if (empty($dados['ico_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['ico_spiid'][$this->obrid][$i]);
                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->icoid = $dados['icoid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['ico_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['ico_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );

                $itemComposicao->carregarPorId($dados['icoid'][$this->obrid][$i]);
                if($itemComposicao->relativoedificacao == 'D')
                $percentExec += $supervisaoItem->spivlritemsobreobraexecanterior;

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $itemComposicao->icopercexecutado = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i]);
                $itemComposicao->salvar();
                $itemComposicao->clearDados();
            }
        }

        $this->percentExec = $percentExec;

        // Salva detalhamento
        if (is_array($dados['dit_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dit_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dit_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dit_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0] ? MoedaToBd($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0]) : NULL);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid'][$this->obrid][$i]);
//                $detalheitem->ditdsc = addslashes($detalheitem->ditdsc);
                $detalheitem->ditdsc = $detalheitem->ditdsc;
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        // Salva detalhamentos Filhos
        if (is_array($dados['dtf_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dtf_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dtf_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dtf_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid_filho'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0] ? MoedaToBd($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0]) : NULL);
                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid_filho'][$this->obrid][$i]);
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        return true;
    }

    public function depoisSalvarVistoriaEmpresa() {
        $dados = $_POST;
        $supervisaoItem = new SupervisaoItem();
        $itemComposicao = new ItensComposicaoObras();
        $detalheitem = new DetalheItem();
//        $estrutura     = new EstruturaObra();
//        // Salva estruturas
//        if ( is_array($dados['edf_spiid']) ){
//            for ($i=0; $i < count($dados['edf_spiid']); $i++){
//                if ( empty($dados['edf_spiid'][$i]) == false )
//                    $supervisaoItem->carregarPorId( $dados['edf_spiid'][$i] );
//
//                $supervisaoItem->spiid                           = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
//                $supervisaoItem->supid                          = $this->supid;
//                $supervisaoItem->eobid                           = $dados['eobid'][$i];
////                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlritemexecanterior'][$i] );
////                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_supvlritemsobreobraexecanterior'][$i] );
//                $supervisaoItem->supvlrfinanceiroanterior        = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['edf_suppercsobreobrainfsupervisor'][$i] == '' ? 0 : $dados['edf_suppercsobreobrainfsupervisor'][$i] );
//                $supervisaoItem->supvlrinfsupervisor             = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i]           == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i]           );
//                $supervisaoItem->supvlrfinanceiroinfsupervisor   = MoedaToBd( $dados['edf_supvlrfinanceiroinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrfinanceiroinfsupervisor'][$i] );
//                $supervisaoItem->salvar();
//                $supervisaoItem->clearDados();
//
//                $estrutura->carregarPorId( $dados['eobid'][$i] );
//                $estrutura->eobpercexecutado = MoedaToBd( $dados['edf_supvlrinfsupervisor'][$i] == '' ? 0 : $dados['edf_supvlrinfsupervisor'][$i] );
//                $estrutura->salvar();
//
//            }
//        }
        $percentExec = 0;
        // Salva etapas
        if (is_array($dados['ico_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['ico_spiid'][$this->obrid]); $i++) {
                if (empty($dados['ico_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['ico_spiid'][$this->obrid][$i]);
                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->icoid = $dados['icoid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['ico_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['ico_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spitotitemsupervisor = MoedaToBd($dados['ico_spitotitemsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spitotitemsupervisor'][$this->obrid][$i]);
//ver($supervisaoItem->supid, $supervisaoItem->spiid, $supervisaoItem->icoid, $supervisaoItem->spitotitemsupervisor);

                $itemComposicao->carregarPorId($dados['icoid'][$this->obrid][$i]);
                if($itemComposicao->relativoedificacao == 'D')
                    $percentExec += $supervisaoItem->spivlritemsobreobraexecanterior;

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $itemComposicao->icopercexecutado = MoedaToBd($dados['ico_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['ico_spivlrinfsupervisor'][$this->obrid][$i]);
                $itemComposicao->salvar();
                $itemComposicao->clearDados();
            }
        }
//        exit;

        $this->percentExec = $percentExec;

        // Salva detalhamento
        if (is_array($dados['dit_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dit_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dit_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dit_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dit_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0] ? MoedaToBd($dados['dit_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid'][$this->obrid][$i]][0]) : NULL);

                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid'][$this->obrid][$i]);
//                $detalheitem->ditdsc = addslashes($detalheitem->ditdsc);
                $detalheitem->ditdsc = $detalheitem->ditdsc;
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dit_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        // Salva detalhamentos Filhos
        if (is_array($dados['dtf_spiid'][$this->obrid])) {
            for ($i = 0; $i < count($dados['dtf_spiid'][$this->obrid]); $i++) {
                if (empty($dados['dtf_spiid'][$this->obrid][$i]) == false)
                    $supervisaoItem->carregarPorId($dados['dtf_spiid'][$this->obrid][$i]);

                $supervisaoItem->spiid = $supervisaoItem->supid != $this->supid ? null : $supervisaoItem->spiid;
                $supervisaoItem->supid = $this->supid;
                $supervisaoItem->ditid = $dados['ditid_filho'][$this->obrid][$i];
//                $supervisaoItem->supvlritemexecanterior          = MoedaToBd( $dados['dit_supvlritemexecanterior'][$this->obrid][$i] );
//                $supervisaoItem->supvlritemsobreobraexecanterior = MoedaToBd( $dados['dit_supvlritemsobreobraexecanterior'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroanterior = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemexecanterior = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlritemsobreobraexecanterior = MoedaToBd($dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spipercsobreobrainfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrinfsupervisor = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrfinanceiroinfsupervisor = MoedaToBd($dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] == '' ? 0 : $dados['dtf_spivlrfinanceiroinfsupervisor'][$this->obrid][$i] );
                $supervisaoItem->spivlrmetafisicasupervisor = ($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0] ? MoedaToBd($dados['dtf_spivlrmetafisicasupervisor'][$this->obrid][$dados['ditid_filho'][$this->obrid][$i]][0]) : NULL);
                $supervisaoItem->salvar();
                $supervisaoItem->clearDados();

                $detalheitem->carregarPorId($dados['ditid_filho'][$this->obrid][$i]);
                $detalheitem->ditpercexecutado = MoedaToBd($dados['dtf_spivlrinfsupervisor'][$this->obrid][$i]);
                $detalheitem->salvar();
                $detalheitem->clearDados();
            }
        }

        return true;
    }

/*
//    public function existeSupervisao( $obrid )
//    {
//        $obrid = ($obrid ? $obrid : $_SESSION['obras2']['obrid']);
//
//        $sql = "SELECT
//                    COUNT(supid) as total
//                FROM
//                    obras2.supervisao
//                WHERE
//                    obrid = {$obrid} AND
//                    supstatus = 'A'";
//
//        return ($this->pegaUm( $sql ) ? true : false);
//    }
//
*/
    /**
     * Verifica se existe vistorias com data maior do que a da que esta tentando excluir
     *
     * @param integer $supid
     * @return bool Existe ou não vistoria com data posterior
     */
    public function verificaExistenciaVistorias($supid, Array $param = array()) {
        $arWhere = array();

        if ($param['not(emsid)']) {
            $arWhere[] = "emsid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "smiid IS NULL";
        }

        $sql = "SELECT
                    supdata
                FROM
                    obras2.supervisao
                WHERE
                    supid = {$supid} AND
                    supstatus = 'A'";

        $datavistoriaatual = $this->pegaUm($sql);

        if (!empty($datavistoriaatual)) {
            $sql = "SELECT
                    count(supid) as total
                FROM
                    obras2.supervisao
                WHERE
                    supdata > '{$datavistoriaatual}' AND
                    supstatus = 'A' AND
                    " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . "
                    obrid = {$_SESSION['obras2']['obrid']}";

            $vistoria = $this->pegaUm($sql);
        }
        if ($vistoria > 0) {
            return false;
        } else {
            return true;
        }
    }

    public function podeatualizarvistoria($supid) {
        global $db;

        if ($db->testa_superuser() || possuiPerfil(PFLCOD_ADMINISTRADOR)) {
            return 'S';
        } else {

            $responsavel = $db->pegaUm("SELECT
	                                        usucpf
	                                    FROM
	                                        obras2.supervisao
	                                    WHERE
	                                        supid = {$supid} AND
	                                        usucpf = '{$_SESSION["usucpf"]}'");

            $retorno = ($responsavel ? 'S' : 'N');

            return $retorno;
        }
    }

    /**
     * Regras de negï¿½cio para ver se pode excluir a vistï¿½ria
     *
     * @param integer $supid - id da vistoria
     * @return bool || string Mensagem de erro caso exista
     */
    public function antesExcluirVistoria($supid) {

        ////// VERIFICA SE A VISTï¿½RIA FOI FEITA PELO USUï¿½RIO LOGADO /////
        $sql = "SELECT count(usucpf) as total 
        		FROM obras2.supervisao
	            WHERE
	                usucpf = '" . $_SESSION['usucpf'] . "' AND
	                supstatus = 'A' AND 
	                supid = " . $supid;

        $boVistoriaDoUsuario = (bool) $this->pegaUm($sql);

        if (!$boVistoriaDoUsuario && !possui_perfil(PFLCOD_SUPER_USUARIO)) {
            return 'Você não pode excluir essa vistoria porque ela não foi feita por você';
        }

        return true;
    }

    public function deletarVistoria($supid, Array $param = array()) {
        $arWhere = array();

        if ($param['not(emsid)']) {
            $arWhere[] = "emsid IS NULL";
        }

        if ($param['not(smiid)']) {
            $arWhere[] = "smiid IS NULL";
        }

        // Executa verificaï¿½ï¿½o para ver se pode excluir a vistï¿½ria de acordo com as regras de negï¿½cio
        $boPodeExcluir = $this->antesExcluirVistoria($supid);
        if (true !== $boPodeExcluir) {
            return $boPodeExcluir;
        }

        $this->percentExec = $this->pegaPercentSupervisao($supid);

        $retorno = $this->carregar("SELECT
                                                sic.icoid as id,
                                                ico.icopercexecutado as executado,
                                                sic.spivlritemsobreobraexecanterior as executadoanterior
                                           FROM
                                                obras2.supervisaoitem sic
                                           INNER JOIN
                                                obras2.itenscomposicaoobra ico ON ico.icoid = sic.icoid
                                           JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus IN ('A', 'H')
                                           WHERE
                                                sic.supid = " . $supid);

        for ($i = 0; $i < count($retorno); $i++) {
            if (!isset($retorno[$i]['executadoanterior'])) {
                $retorno[$i]['executadoanterior'] = 0;
            }
            if (!isset($retorno[$i]['executado'])) {
                $retorno[$i]['executado'] = 0;
            }

            if ($retorno[$i]["id"]) {
                $sql = $this->executar("
                                UPDATE
                                    obras2.itenscomposicaoobra
                                SET
                                    icopercexecutado = " . $retorno[$i]['executadoanterior'] . "
                                WHERE
                                    icoid = " . $retorno[$i]['id']);
            }

            $total += $retorno[$i]['executadoanterior'];
        }

//        if ( !empty($total)){
//            $sql = "";
//            $sql = "UPDATE
//                        obras.obrainfraestrutura
//                    SET
//                        obrpercexec = {$total}
//                    WHERE
//                        obrid = {$_SESSION['obras2']["obrid"]}";
//
//            $this->executar($sql);
//        }
        // exclui os itens da supervisao
        $this->executar("DELETE FROM obras2.supervisaoitem WHERE supid = " . $supid);

        // deleta as fotos da vistoria
        $this->executar("DELETE FROM obras2.fotos WHERE supid = " . $supid);

        // desativa a vistoria
        $desativaVistoria = $this->executar("UPDATE obras2.supervisao SET supstatus = 'I' WHERE supid = " . $supid);

        //Quando desativada a vistoria, a data da ï¿½ltima vistoria serï¿½ inserida na tabela "obras.obrainfraestrutura", no campo "obrdtvistoria".
        if ($desativaVistoria) {
            //Seleciona a ï¿½ltima Vistoria, quando desativada a Vistoria mais recente.
            $sqlDtinclusao = " SELECT
                                    supdata
                               FROM
                                    obras2.supervisao
                               WHERE
                                    obrid = " . $_SESSION['obras2']['obrid'] . " AND
                                    " . (count($arWhere) ? implode(" AND ", $arWhere) . " AND " : "") . " 
                                    supstatus = 'A'
							   ORDER BY supid DESC ";
            $dtInclusao[0]['supdata'] = $this->pegaUm($sqlDtinclusao);
            //Atualiza a data da Lista de Obras, quando a vistoria for desativada, com a data da penï¿½ltima vistoria inserida.
            if ($dtInclusao[0]['supdtinclusao'] != '') {
                $sqlAtualizaDtInclusao = " UPDATE
                                            obras2.obras
                                       SET
                                            obrdtvistoria ='" . $dtInclusao[0]['supdata'] . "'
                                       WHERE
                                            obrid = {$_SESSION['obras2']['obrid']} ";
            } else {
                $sqlAtualizaDtInclusao = " UPDATE
	                                            obras2.obras
	                                       SET
	                                            obrdtvistoria = NULL
	                                       WHERE
	                                            obrid = {$_SESSION['obras2']['obrid']} ";
            }
            $this->executar($sqlAtualizaDtInclusao);
//          ver($desativaVistoria,$supid,$_SESSION['obras2']["obrid"],$dtInclusao[0]['supdtinclusao'],d);
        }

        // atualiza a situaï¿½ï¿½o da obra
        $staid = $this->pegaSituacaoVistoria($_SESSION['obras2']['obrid']); //projeto_pega_situacao_vistoria( $_SESSION['obras2']['obrid'] );

        if ($staid) {
            $sql = "UPDATE
                        obras2.obras SET staid = {$staid}
                    WHERE
                        obrid = {$_SESSION['obras2']['obrid']}";

            $this->pegaUm($sql);
        }

//        // Verifica se a Vistoria foi Cadastrada por uma Empresa.
//        $empresa = verifica_realizado_por_empresa($_SESSION['obras2']["obrid"]);
//
//        if($empresa || $staid){
//
//            //Caso a Vistoria for Cadastrada por uma Empresa serï¿½ atribuï¿½da a Situaï¿½ï¿½o da Vistoria.
//            $stoidEmpresa = pega_situacao_obra_empresa($_SESSION['obras2']["obrid"]);
//
//            //Caso a Vistoria for Cadastrada por uma Empresa serï¿½ atribuï¿½do o Percentual  Executado da Supervisï¿½o.
//            $obrsuppercexecEmpresa = pega_percentual_obra_empresa($_SESSION['obras2']["obrid"]);
//
//            if( $obrsuppercexecEmpresa && $stoidEmpresa ){
//                $empresaPercentualSituacao = " obrsuppercexec = {$obrsuppercexecEmpresa} ,
//                                                stoidsupemp = {$stoidEmpresa} ";
//            }else{
//                $empresaPercentualSituacao = " obrsuppercexec = NULL ,
//                                                stoidsupemp = NULL ";
//            }
//            $sql = "UPDATE
//                        obras.obrainfraestrutura
//                        SET {$empresaPercentualSituacao}
//                    WHERE
//                        obrid = {$_SESSION['obras2']["obrid"]}";
//
//            $this->pegaUm($sql);
//        }
        // verifica se existe histï¿½rico de paralisaï¿½ï¿½o da obra
        $sql = "SELECT hprid
                FROM obras2.historicoparalisacao
                WHERE obrid = {$_SESSION['obras2']['obrid']} AND hprdtstatus = 'A'";

        $hprid = $this->pegaUm($sql);

        // inavita o histï¿½rico
        if (!empty($hprid)) {

            $this->executar("UPDATE obras2.historicoparalisacao
                                    SET hprdtstatus = 'I' WHERE hprid = {$hprid}");
        }

        $this->carregarPorId($supid);

        $situacaoAtividade = new SituacaoAtividade($this->staid);

        $regAtividade = new RegistroAtividade();
        $arDado['obrid'] = $this->obrid;
        $arDado['usucpf'] = $_SESSION['usucpf'];
        $arDado['rgaautomatica'] = 't';
        $arDado['rgadscsimplificada'] = 'VISTORIA APAGADA';
        $arDado['rgadsccompleta'] = "ID acompanhamento: {$this->supid}
Data do acompanhamento: " . formata_data($this->supdata) . "
CPF: {$_SESSION['usucpf']}
Situação: {$situacaoAtividade->stadesc}
% do acompanhamento: {$this->percentExec}
Quando: " . date('d/m/Y');
        $regAtividade->popularDadosObjeto($arDado)
                ->salvar();

        $this->commit();

        $this->sucesso('principal/vistoria');
        return true;
    }

    public function resgataTodosRsuidsPossiveis() {
        $sql = "
    		SELECT rsuid, rsudsc
    		FROM obras2.realizacaosupervisao
    		ORDER BY rsuid
    	";
        return $this->carregar($sql);
    }

    public function getLinksArquivosArt($supid) {
        
        if($supid == null){
            return array();
        }else{
            $sql = " SELECT 
                        '<a href=\"javascript:void(0);\" 
                                id=\"' || sup.arquid_art_exec || '\" 
                                class=\"download\" 
                                title=\"Baixar Arquivo\">' 
                                || a1.arqnome || '.' || a1.arqextensao || 
                        '</a>' as arq_art_exec,
                        '<a href=\"javascript:void(0);\" 
                                id=\"' || sup.arquid_art_fisc || '\" 
                                class=\"download\" 
                                title=\"Baixar Arquivo\">' 
                                || a2.arqnome || '.' || a2.arqextensao || 
                        '</a>' as arq_art_fisc    
                     FROM obras2.supervisao sup
                     INNER JOIN public.arquivo  a1 ON a1.arqid = sup.arquid_art_exec  
                     INNER JOIN public.arquivo  a2 ON a2.arqid = sup.arquid_art_fisc
                     WHERE sup.supid = ".$supid."

                    ";
            $dados = $this->carregar($sql);
            return ($dados ? $dados : array());        
        }
    }
    
    public function registroDeMudancaDeArquivoArt($arrArquivosArt, $obrid = null) {
        
        $rgadscsimplificada = 'Alteração do arquivo ';
        $plural  = '';
        $plural2 = 'foi';
        
        if($arrArquivosArt['arquivo_art_exec'] != '') {
            $rgadscsimplificada .= 'anexo ART de Execução';
        }
        
        if($arrArquivosArt['arquivo_art_exec'] != '' && $arrArquivosArt['arquivo_art_fisc'] != '') {
            $rgadscsimplificada .= ' e ';
            $plural  = 's';
            $plural2 = 'foram';
        }
        
        if($arrArquivosArt['arquivo_art_fisc'] != '') {
            $rgadscsimplificada .= 'anexo ART de Fiscalização';
        }
        
        $rgadscsimplificada .= '.';
        
        $rgadsccompleta  = 'O'.$plural.' arquivo'.$plural.' de ID'.$plural.' '.  implode(',', $arrArquivosArt).' '.$plural2.' modificado'.$plural.'.';
        $rgadsccompleta .= " CPF: {$_SESSION['usucpf']}. ";
        $rgadsccompleta .= " Quando: " . date('d/m/Y');
        
        $regAtividade                 = new RegistroAtividade();
        $arDado                       = array();
        $arDado['obrid']              = ($obrid != null) ? $obrid : $this->obrid;
        $arDado['usucpf']             = $_SESSION['usucpf'];
        $arDado['rgaautomatica']      = 't';
        $arDado['rgadscsimplificada'] = $rgadscsimplificada;
        $arDado['rgadsccompleta']     = $rgadsccompleta;

        try{            
            $regAtividade->popularDadosObjeto($arDado);
            $regAtividade->salvar();



            return true;
        }catch (Exception $ex) {
            return false;
        }
        
    }
    
    public function getValidadeSupUnidade($obrid, $supid) {
        
       $sql = " SELECT                     
                    s.supid,
                    s.usucpf,
                    validadaPeloSupervisorUnidade
                FROM
                    obras2.supervisao s                
                WHERE
                    s.obrid = '" . $obrid . "' AND
                    s.supid = '" . $supid . "' AND
                    s.supstatus = 'A'
              ";

        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
        
    }
    
    public function getUltimaParalicaoObra($obrid)
    {
        $strSQL = "SELECT 
            distinct to_char(s.supdata,'DD/MM/YYYY') as dtvistoria, 
            to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao, 
            COALESCE('0', '-') AS aditivo, 
            u.usunome, 
            si.staid,
            si.stadesc, 
            rs.rsudsc as responsavel, 
            UPPER(e.entnome) as vistoriador, 
            s.supid, 
            s.usucpf, 
            e.entnumcpfcnpj as cpfvistoriador, 
            s.supdtinclusao, 
            (SELECT 
                    CASE WHEN SUM(icovlritem) > 0 THEN ROUND( (SUM( spivlrfinanceiroinfsupervisor ) / SUM(icovlritem)) * 100, 2) 
                    ELSE 0
                    END AS total 
                    -- SUM(spivlritemsobreobraexecanterior) AS total 
            FROM obras2.itenscomposicaoobra i 
            LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
            JOIN obras2.supervisao su ON su.supid = sic.supid
            JOIN obras2.cronograma cro ON cro.croid = su.croid AND cro.crostatus IN ('A', 'H') AND i.croid = cro.croid AND cro.obrid = i.obrid
            WHERE i.icostatus = 'A' AND i.obrid = '18329' ) as percentual,
            doc.esdid, 
            s.supdata, 
            s.supid_1, 
            rs.rsuid as realizacaosupervisao_rsuid, 
            CASE WHEN validadaPeloSupervisorUnidade = 'N' OR validadaPeloSupervisorUnidade IS NULL 
            THEN 'Não' WHEN validadaPeloSupervisorUnidade = 'S' THEN 'Sim' 
            END as validado 
            FROM obras2.supervisao s 
            INNER JOIN seguranca.usuario u ON u.usucpf = s.usucpf 
            LEFT JOIN obras2.situacaoatividade si ON si.staid = s.staid 
            LEFT JOIN entidade.entidade e ON e.entid = s.supvistoriador 
            LEFT JOIN obras2.realizacaosupervisao rs ON rs.rsuid = s.rsuid 
            LEFT JOIN workflow.documento doc ON doc.docid = s.docid 
            WHERE 
                    s.obrid = '{$obrid}' AND 
                    s.emsid IS NULL AND 
                    s.smiid IS NULL AND 
                    s.supstatus = 'A' 
            ORDER BY 
                    s.supdata DESC";
        
        $dados = $this->carregar($strSQL);

        if (!empty($dados)) {
            /**
             * Pega sempre a primeira paralisação da obra,
             * usa o contador $i para identificar, 
             * no caso de paralisações sequenciais
             * pegar sempre a mais antiga
             */
            $return = null;
            $i = 0;
            foreach ($dados as $row) {
                $i++;
                if ($row['staid'] == 2 && $i == 1) {
                    $return = $row;
                    $i = 0;
                }
            }
            return $return['dtvistoria'];
        }
        
        return '';
    }
    
    public function pegaUltimaVistoriaUnidade($obrid)
    {
        $sql = "SELECT distinct
                to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,
                COALESCE('0', '-') AS aditivo,
                u.usunome,
                si.stadesc,
                rs.rsudsc as responsavel,
                UPPER(e.entnome) as vistoriador,
                s.supid,
                s.usucpf,
                e.entnumcpfcnpj as cpfvistoriador,
                s.supdtinclusao,
                ( SELECT
                        CASE 
                            WHEN SUM(icovlritem) > 0 THEN 
                                ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                            ELSE 
                                0	
                        END AS total
                                -- SUM(spivlritemsobreobraexecanterior) AS total
                    FROM
                        obras2.itenscomposicaoobra i
                    LEFT JOIN
                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                        AND sic.supid = s.supid
                        AND sic.icoid IS NOT NULL
                        AND sic.ditid IS NULL
                    JOIN obras2.supervisao su ON su.supid = sic.supid
                    JOIN obras2.cronograma cro ON cro.croid = su.croid AND cro.crostatus IN ('A', 'H') AND i.croid = cro.croid AND cro.obrid = i.obrid

                    WHERE
                        i.icostatus = 'A' AND
                        i.obrid = '{$obrid}'
                ) as percentual,
                doc.esdid,
                s.supdata,
                s.supid_1,
                rs.rsuid as realizacaosupervisao_rsuid,
                CASE 
                    WHEN validadaPeloSupervisorUnidade = 'N' OR validadaPeloSupervisorUnidade IS NULL THEN '<font style=\"color:red\">Não</font>'
                    WHEN validadaPeloSupervisorUnidade = 'S' THEN '<font style=\"color:green\">Sim</font>'
                END as validado  
            FROM
                obras2.supervisao s
            INNER JOIN
                seguranca.usuario u ON u.usucpf = s.usucpf
            LEFT JOIN
                obras2.situacaoatividade si ON si.staid = s.staid
            LEFT JOIN
                entidade.entidade e ON e.entid = s.supvistoriador
            LEFT JOIN
                obras2.realizacaosupervisao rs ON rs.rsuid = s.rsuid
            LEFT JOIN
                workflow.documento doc ON doc.docid = s.docid
            WHERE
                s.obrid = '{$obrid}'
                AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.usucpf IS NOT NULL AND s.rsuid = 1
            ORDER BY
                supdata DESC LIMIT 1";
        
        //dbg(simec_htmlentities($sql), d);
        
        $dados = $this->pegaLinha($sql);
        
        return (count($dados)) ? $dados : array();
    }
    

    public function pegaVistoriaUnidade($supid)
    {
        $sql = "SELECT distinct
                to_char(s.supdata,'DD/MM/YYYY') as dtvistoria,
                to_char(s.supdtinclusao,'DD/MM/YYYY') as dtinclusao,
                COALESCE('0', '-') AS aditivo,
                u.usunome,
                si.stadesc,
                rs.rsudsc as responsavel,
                UPPER(e.entnome) as vistoriador,
                s.supid,
                s.usucpf,
                e.entnumcpfcnpj as cpfvistoriador,
                s.supdtinclusao,
                ( SELECT
                        CASE
                            WHEN SUM(icovlritem) > 0 THEN
                                ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                            ELSE
                                0
                        END AS total
                                -- SUM(spivlritemsobreobraexecanterior) AS total
                    FROM
                        obras2.itenscomposicaoobra i
                    LEFT JOIN
                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                        AND sic.supid = s.supid
                        AND sic.icoid IS NOT NULL
                        AND sic.ditid IS NULL
                    JOIN obras2.supervisao su ON su.supid = sic.supid
                    JOIN obras2.cronograma cro ON cro.croid = su.croid AND cro.crostatus IN ('A', 'H') AND i.croid = cro.croid AND cro.obrid = i.obrid

                    WHERE
                        i.icostatus = 'A' AND
                        i.obrid = s.obrid
                ) as percentual,
                doc.esdid,
                s.supdata,
                s.supid_1,
                rs.rsuid as realizacaosupervisao_rsuid,
                CASE
                    WHEN validadaPeloSupervisorUnidade = 'N' OR validadaPeloSupervisorUnidade IS NULL THEN '<font style=\"color:red\">Não</font>'
                    WHEN validadaPeloSupervisorUnidade = 'S' THEN '<font style=\"color:green\">Sim</font>'
                END as validado
            FROM
                obras2.supervisao s
            INNER JOIN
                seguranca.usuario u ON u.usucpf = s.usucpf
            LEFT JOIN
                obras2.situacaoatividade si ON si.staid = s.staid
            LEFT JOIN
                entidade.entidade e ON e.entid = s.supvistoriador
            LEFT JOIN
                obras2.realizacaosupervisao rs ON rs.rsuid = s.rsuid
            LEFT JOIN
                workflow.documento doc ON doc.docid = s.docid
            WHERE
                s.supid = '{$supid}'
                AND s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.usucpf IS NOT NULL AND s.rsuid = 1
            ";

        //dbg(simec_htmlentities($sql), d);

        $dados = $this->pegaLinha($sql);

        return (count($dados)) ? $dados : array();
    }

}
