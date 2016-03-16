<?php

class Ted_Utils_Model extends Ted_Connect
{

    /**
     * @return array|void
     */
    public static function pegaResponsavelPolitica()
    {
        $strSQL = "
            SELECT
                dircod || '_dircod' as codigo,
                ''||ug.ungabrev||' / ' || dirdsc as descricao
            FROM public.unidadegestora ug
                inner join ted.diretoria d ON d.ungcod = ug.ungcod
            WHERE ungstatus='A' and dirstatus = 'A'
                  AND d.dircod IN (38,39,41,42,43,58)
            UNION ALL
            SELECT
                ungcod || '_ungcod' as codigo,
                ungabrev||' / '||ungdsc as descricao
            FROM public.unidadegestora
            WHERE ungstatus = 'A'
                  AND ungcod in ('150019','150028','150016')
        ";

        return self::dbGetInstance()->carregar($strSQL);
    }

    public static function pegaUF()
    {
        $strSQL = "
            SELECT
                estuf as codigo,
                estdescricao as descricao
            FROM
                territorios.estado
            ORDER BY 2
        ";

        $list = self::dbGetInstance()->carregar($strSQL);
        $options = array();

        if ($list) {
        	foreach($list as $item) {
        		$options[$item['codigo']] = $item['descricao'];
        	}
        }        
        return ($options) ? $options : null;
    }
    
    /**
     * @param null $estuf
     * @return array|void
     */
    public static function pegaMunicipio($estuf = null)
    {
        if (null !== $estuf) {
            $strSQL = "
                SELECT 	muncod as codigo,
                        mundescricao as descricao
                FROM territorios.municipio
                WHERE estuf ilike '".$estuf."'
                ORDER BY 2
			";
        } else {
            $strSQL = "SELECT '' as codigo, 'Selecione uma UF' as descricao";
        }
       	$list = self::dbGetInstance()->carregar($strSQL);
        $options = array();
        if ($list) {
        	foreach($list as $item) {
        		$options[$item['codigo']] = $item['descricao'];
        	}
        }        
        return ($options) ? $options : null;
    }

    /**
     * @param $tpcid
     * @return bool|string|void
     */
    public static function getDocid($tpcid)
    {
        $tpcid = preg_replace('/[^0-9]/', '', $tpcid);

        $strSQL = "
            SELECT docid FROM
            ted.termocompromisso
            WHERE tcpid = {$tpcid}
        ";

        return self::dbGetInstance()->pegaUm($strSQL);
    }

    /**
     * @return array|void
     */
    public static function pegaEstadosTermo()
    {
        $strSQL =  "
            SELECT
                esdid AS codigo, esddsc AS descricao
            FROM workflow.estadodocumento
            WHERE
                tpdid = ".WF_TPDID_DESCENTRALIZACAO." AND
                esdstatus = 'A'
            ORDER BY esdordem
        ";

        $list = self::dbGetInstance()->carregar($strSQL);
        $options = array();
        if ($list) {
            foreach($list as $item) {
                $options[$item['codigo']] = $item['descricao'];
            }
        }

        return ($options) ? $options : null;
    }

    /**
     * @param $redirect = false, caso queira redirecionar utilize true
     * @return mixed
     */
    public static function capturaTcpid($redirect = false)
    {
    	if (!Ted_Utils_Model::isTcpId() && $redirect) {
            echo '<script type="text/javascript">
    		    alert("Não foi possível encontrar o código do termo, tente novamente.");
    		    document.location.href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A";
    		</script>';
            die();
        }
		
        if(!Ted_Utils_Model::isTcpId()){
        	return false;
        }
        
        return (int) ($_GET['ted'])? preg_replace('/[^0-9]/', '', $_GET['ted']) : false;
    }

    /**
     * Verifica o tcpid para efetuar (ou não) o redirecionamento
     */
    public static function isTcpId()
    {
    	if (isset($_GET['ted']) && trim($_GET['ted']) != '') {
            $tcpid = (int) preg_replace('/[^0-9]/', '', $_GET['ted']);
    		$sql = "select 1 from ted.termocompromisso where tcpid = {$tcpid}";
    		if (self::dbGetInstance()->carregar($sql)) {
    			return true;
    		}
    	}

    	return false;
    }
    
    
    /**
     * Monta painel com informações sobre o termo da sessão.
     */
    public static function montaInformacaoTermo()
    {
    	$tcpid = Ted_Utils_Model::capturaTcpid();
    	if (!$tcpid) return false;

        $strSQL = "
            SELECT
                unp.ungcod ||' / '|| unp.ungdsc as unidade,
                esd.esddsc as situacao
            FROM ted.termocompromisso tcp
                INNER JOIN public.unidadegestora unp      ON unp.ungcod = tcp.ungcodproponente
                INNER JOIN workflow.documento doc         ON doc.docid = tcp.docid
                INNER JOIN workflow.estadodocumento esd   ON esd.esdid = doc.esdid
            WHERE tcp.tcpid = {$tcpid}
        ";
        $dado = self::dbGetInstance()->pegaLinha($strSQL);

        $strSQL = "
            select count(*) from workflow.historicodocumento hst where hst.aedid = 1620 and hst.docid = (
                select docid from ted.termocompromisso where tcpid = {$tcpid}
            )
        ";
        $countAlteracoes = self::dbGetInstance()->pegaUm($strSQL);
        $tcpid .= ($countAlteracoes) ? ".{$countAlteracoes}" : '';
        $ted = new Ted_Model_TermoExecucaoDescentralizada($tcpid);

        $strSQL = sprintf("
            select max(tcpnumtransfsiafi) from ted.previsaoparcela where proid in (
                select proid from ted.previsaoorcamentaria where tcpid = %d and prostatus = 'A'
            )
        ", $tcpid);
        $tcpnumtransfsiafi = self::dbGetInstance()->pegaUm($strSQL);

        $tpl = "
            <div class=\"panel panel-info\">
                <div class=\"panel-heading\">
                    <span class=\"glyphicon glyphicon-info-sign\"></span><strong> Número do TED SIMEC</strong> <code>{$tcpid}</code>
                    %s
                </div>
                <ul class=\"list-group\">
                    <li class=\"list-group-item\"><strong>Unidade Gestora Proponente</strong> {$dado['unidade']}</li>
                    <li class=\"list-group-item\"><strong>Situação</strong> <small class=\"text-info situacao-ted\"><kbd style='color:#3A87AD;'>{$dado['situacao']}</kbd></small></li>
                    %s
                    %s
                </ul>
            </div>
        ";

        if ($tcpnumtransfsiafi) {
            $transferencia = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <span class=\"glyphicon glyphicon-info-sign\"></span><strong> Número de Transferência SIAFI</strong> <code>{$tcpnumtransfsiafi}</code>";
        } else {
            $transferencia = '';
        }

        if ($ted->recuperaNumeroProcessoFNDE()) {
            $numProcesso = '<li class="list-group-item"><strong>Número do Processo</strong> '.$ted->recuperaNumeroProcessoFNDE().'</li>';
        } else {
            $numProcesso = '';
        }

        if (Ted_Utils_Model::pegarVigenciaTED()) {
            $vigencia = '<li class="list-group-item"><strong>Vigência</strong> '.Ted_Utils_Model::pegarVigenciaTED().'</li>';
        } else {
            $vigencia = '';
        }

        echo sprintf($tpl, $transferencia, $numProcesso, $vigencia);
    }
    
    public static function mes_extenso($mes)
    {
    	if (strval($mes) == 1) return 'JANEIRO';
    	else   if (strval($mes) == 2) return 'FEVEREIRO';
    	else   if (strval($mes) == 3) return 'MARÇO';
    	else   if (strval($mes) == 4) return 'ABRIL';
    	else   if (strval($mes) == 5) return 'MAIO';
    	else   if (strval($mes) == 6) return 'JUNHO';
    	else   if (strval($mes) == 7) return 'JULHO';
    	else   if (strval($mes) == 8) return 'AGOSTO';
    	else   if (strval($mes) == 9) return 'SETEMBRO';
    	else   if (strval($mes) == 10) return 'OUTUBRO';
    	else   if (strval($mes) == 11) return 'NOVEMBRO';
    	else   if (strval($mes) == 12) return 'DEZEMBRO';
    }

    /**
     * @return string
     */
    public static function recuperaDataGeraPdf()
    {
    	$tcpid = Ted_Utils_Model::capturaTcpid();
    
    	$sql = "
    		SELECT
    			MAX(hd.htddata) AS data
    		FROM workflow.historicodocumento hd
    		INNER JOIN workflow.acaoestadodoc ac ON ac.aedid = hd.aedid
    		INNER JOIN workflow.estadodocumento ed ON ed.esdid = ac.esdidorigem
    	    INNER JOIN seguranca.usuario us ON us.usucpf = hd.usucpf
    		LEFT JOIN workflow.comentariodocumento cd ON cd.hstid = hd.hstid
    		WHERE 
    			hd.docid = (SELECT docid FROM ted.termocompromisso WHERE tcpid = {$tcpid})
    			AND ac.esdidorigem IN (635)
    	";
    
    	$rs = !empty($tcpid) ? self::dbGetInstance()->pegaUm($sql) : false;
    
    	if ($rs) {
	    	$arData = explode(' ', $rs);
	    	$arData = explode('-', $arData[0]);
	    
	    	switch ($arData[1]) {
	    		case 1: $mes = "Janeiro"; break;
	    		case 2: $mes = "Fevereiro"; break;
	    		case 3: $mes = "Março"; break;
	    		case 4: $mes = "Abril"; break;
	    		case 5: $mes = "Maio"; break;
	    		case 6: $mes = "Junho"; break;
	    		case 7: $mes = "Julho"; break;
	    		case 8: $mes = "Agosto"; break;
	    		case 9: $mes = "Setembro"; break;
	    		case 10: $mes = "Outubro"; break;
	    		case 11: $mes = "Novembro"; break;
	    		case 12: $mes = "Dezembro"; break;
	    
   			}	    
	    	return 'Brasília, '.$arData[2].' de '.$mes.' de '.$arData[0].'';
    	}
    	return 'Nada Encontrado';
    }

    /**
     * @return bool
     */
    public static function isAjax()
    {
        return (!empty($_SERVER['HTTP_X_REQUESTED_WITH'])
                && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
    }

    /**
     * @param $message
     * @return void(0)
     */
    public static function reloadPage($message, $url)
    {
        $template = '<script type="text/javascript">alert("%s");location.href="%s";</script>';
        echo sprintf($template, $message, $url);
    }
    
    /**
    * Verifica se o termo possui pendencia do retorno do identificador
    * retorna true (sim) ou false (não)
    * @return bool
    */
    public static function verificaEfetivacaoNCSigef()
    {
        $tcpid = Ted_Utils_Model::capturaTcpid();
        $sql = "
            SELECT
                COUNT(*) AS pendencia
            FROM ted.previsaoorcamentaria
            WHERE
                sigefid is not null and
                codsigefnc is null and
                tcpid = {$tcpid}
        ";

        //ver($sql, d);
        $result = self::dbGetInstance()->pegaUm($sql);
        return (boolean) $result;
    }

    /**
     * @param $valor
     * @return string
     */
    public static function formatDateUs($valor)
    {
        if (strlen($valor)) {
            $datePart = explode('/', $valor);
            return "{$datePart[2]}-{$datePart[1]}-{$datePart[0]}";
        }
    }

    /**
     * Valida formato da data, bem como se a data é valida dentro do calendario gregoriano
     * Valida data
     * @param $date
     * @return bool
     */
    public static function dateIsValid($date)
    {
        if (strlen($date)) {
            $datePart = explode('/', $date);
            if (count($datePart)===3) {
                return (checkdate($datePart[1], $datePart[0], $datePart[2])) ? true : false;
            }
        }

        return false;
    }

    /**
     * Executa o redirecionamento usando javascript
     * @param $urlbase
     * @param array $params
     * @return string
     */
    public static function redirect($urlbase ,array $params = array())
    {
        $qString = '';
        if (count($params)) {
            $qString = http_build_query($params, '', '&');
        }

        $jsTpl = "<script type='text/javascript'> location.href = '%s&%s';</script>";
        return sprintf($jsTpl, $urlbase, $qString);
    }

    /**
     * Remove caracteres acentuados da string
     * @param $str
     * @return string
     */
    public static function removeAcento($str)
    {
        $str = utf8_decode($str);
        $from = "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ";
        $to = "aaaaeeiooouucAAAAEEIOOOUUC";

        return strtr($str, $from, $to);
    }

    /**
     * Não permitir exclusão após o arquivo ter sido aprovado pelo Proponente
     * @param $tcpid
     * @return bool
     */
    public static function possuiHistoricoExecucao($tcpid)
    {
        $strSQL = "
            select
                u.usunome,
                u.usucpf,
                to_char(h.htddata, 'DD/MM/YYYY') as htddata,
                to_char(h.htddata, 'HH:II:SS') as hora,
                g.ungdsc
            from ted.termocompromisso t
            inner join workflow.historicodocumento h on h.docid = t.docid
            inner join workflow.acaoestadodoc a on a.aedid = h.aedid
            inner join seguranca.usuario u on u.usucpf = h.usucpf
            left join unidadegestora g on g.ungcod = t.ungcodconcedente
            where
                t.tcpid = {$tcpid} and
                a.esdiddestino in (".EM_ANALISE_DA_SECRETARIA.",".EM_ANALISE_OU_PENDENTE.")
            order by hstid asc
        ";
        //ver($strSQL);
        $result = self::dbGetInstance()->carregar($strSQL);
        return ($result) ? true : false;
    }

    /**
     * Pega Situação do termo chave e descricao
     * @return array|void
     */
    public static function pegaSituacaoTed()
    {
        $strSQL = sprintf("
            SELECT
                doc.esdid, esd.esddsc
            FROM ted.termocompromisso tcp
                inner JOIN workflow.documento doc         ON doc.docid = tcp.docid
                inner JOIN workflow.estadodocumento esd   ON esd.esdid = doc.esdid
            WHERE tcp.tcpid = %d
        ", self::capturaTcpid());

        return self::dbGetInstance()->pegaLinha($strSQL);
    }

    /**
     * @param $pflcod
     * @return bool
     */
    public static function possuiPerfil($pflcod)
    {
        if (!is_array($pflcod)) {
            $pflcod = array($pflcod);
        }

        $strSQL = "
            SELECT
				count(1)
			FROM
				seguranca.perfilusuario p
				JOIN ted.usuarioresponsabilidade u ON (u.usucpf = p.usucpf)
			WHERE
				u.usucpf = '{$_SESSION['usucpf']}'
				AND u.pflcod in ('".implode('\',\'', $pflcod)."')
				AND u.rpustatus = 'A'
        ";
        //ver($strSQL, d);

        return (boolean) self::dbGetInstance()->pegaUm($strSQL);
    }

    /**
     * @return array|void
     */
    public static function pegaPerfilUsuario()
    {
        $strSQL = "
            select pflcod from seguranca.perfil where sisid = 194 and pflcod in (
                    SELECT pflcod FROM seguranca.perfilusuario WHERE usucpf = '{$_SESSION['usucpf']}'
            )
        ";

        return self::dbGetInstance()->carregar($strSQL);
    }

    /**
     * @param array $collection
     * @return bool|string
     */
    public static function arrayToString(array $collection)
    {
        $str = '';
        if (count($collection)) {
            foreach ($collection as $current) {
                $str .= $current['pflcod'].',';
            }

            return substr($str, 0 ,-1);
        }

        return false;
    }

    /**
     * @return bool
     */
    public static function termoTipoEmenda()
    {
        $strSQL = sprintf("
            SELECT tipoemenda FROM ted.justificativa WHERE tcpid = %d
        ", self::capturaTcpid());

        $tipoemenda = self::dbGetInstance()->pegaUm($strSQL);
        return ($tipoemenda == 'S') ? true : false;
    }

    /**
     * @return bool
     */
    public static function concedenteFNDE()
    {
        $strSQL = sprintf("
            SELECT ungcodconcedente FROM ted.termocompromisso WHERE tcpid = %d
        ", self::capturaTcpid());

        $ungcodconcedente = self::dbGetInstance()->pegaUm($strSQL);
        return ($ungcodconcedente == UG_FNDE) ? true : false;
    }

    /**
     * @return bool
     */
    public static function possuiHistorico()
    {
        $strSQL = sprintf("
            SELECT count(*) FROM ted.historico_termocompromisso WHERE tcpid = %d
        ", self::capturaTcpid());

        $existe = self::dbGetInstance()->pegaUm($strSQL);
        return ($existe) ? true : false;
    }

    /**
     * @return bool
     */
    public static function showRCO()
    {
        $situacoes = array(
            EM_EXECUCAO,
            RCO_AGUARDANDO_ANALISE_GESTOR_ORCAMENTARIO_PROPONENTE,
            RCO_AGUARDANDO_ANALISE_REPRESENTANTE_LEGAL_PROPONENTE,
            RCO_AGUARDANDO_ANALISE_DIRETORIA,
            RCO_AGUARDANDO_ANALISE_SECRETARIO,
            RCO_AGUARDANDO_ANALISE_REPRESENTANTE_LEGAL_CONCEDENTE,
            RCO_AGUARDANDO_ANALISE_DIGAP,
            TERMO_EM_DILIGENCIA_RELATORIO,
            RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD,
            TERMO_FINALIZADO
        );

        $situacaoAtual = self::pegaSituacaoTed();
        if (in_array($situacaoAtual['esdid'], $situacoes)) {
            return true;
        } else {
            return self::rcoPendenciaPreenchimento();
        }
    }

    /**
     * Verifica pendencia de preenchimento do RCO
     * se Prazo de preenchimento venceu
     * @return bool
     */
    public static function rcoPendenciaPreenchimento()
    {
        if (self::isTcpId()) {
            $business = new Ted_Model_RelatorioCumprimento_Business();
            if ($business->termoVencido(self::capturaTcpid()) && self::uoEquipeTecnicaProponente()) {
                return true;
            }
        }

        return false;
    }

    /**
     * @return bool
     */
    public static function uoEquipeTecnicaProponente()
    {
        if (self::possuiPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_UG_REPASSADORA, PERFIL_CGSO))) {
            return true;
        }

        if (self::possuiPerfil(array(UO_EQUIPE_TECNICA))) {

            $strSQL = sprintf("
                select * from ted.usuarioresponsabilidade
                where
                    usucpf = '{$_SESSION['usucpf']}' and
                    pflcod = ".UO_EQUIPE_TECNICA." and
                    ungcod = (select ungcodproponente from ted.termocompromisso where tcpid = %d)
            ", self::capturaTcpid());

            $linha = self::dbGetInstance()->pegaLinha($strSQL);
            return ($linha) ? true : false;
        }

        return false;
    }

    /**
     * @return bool
     */
    public static function uoEquipeTecnicaConcedente()
    {
        if (self::possuiPerfil(array(PERFIL_SUPER_USUARIO))) {
            return true;
        }

        if (self::possuiPerfil(array(UO_EQUIPE_TECNICA))) {

            /**
             * Se o termo for FNDE
             * e politica for alguma secretaria (SETEC, SECAD, SEB)
             * pega como codigo concedente o codigo da UG da secretaria
             */
            $secretarias = array(
                '150016',
                '150028',
                '150019',
            );

            $sqlComplement = sprintf("(select ungcodconcedente from ted.termocompromisso where tcpid = %d)", self::capturaTcpid());
            $rsSec = self::dbGetInstance()->pegaUm(sprintf("select ungcodpoliticafnde from ted.termocompromisso where tcpid = %d", self::capturaTcpid()));
            //ver($rsSec);
            if ($rsSec) {
                if (in_array($rsSec, $secretarias)) {
                    $sqlComplement = sprintf("(select ungcodpoliticafnde from ted.termocompromisso where tcpid = %d)", self::capturaTcpid());
                }
            }

            $strSQL = "
                select * from ted.usuarioresponsabilidade
                where
                    usucpf = '{$_SESSION['usucpf']}' and
                    pflcod = ".UO_EQUIPE_TECNICA." and
                    rpustatus = 'A' and
                    ungcod = {$sqlComplement}
            ";

            //ver($strSQL);
            $linha = self::dbGetInstance()->pegaLinha($strSQL);
            return ($linha) ? true : false;
        }

        return false;
    }

    public static function userIsAllowed()
    {
        $strSQL = "
            SELECT t.tcpid
            FROM ted.termocompromisso t
            LEFT JOIN ted.coordenacao cdn ON cdn.cooid = t.cooid
            LEFT JOIN public.unidadegestora unp ON unp.ungcod = t.ungcodproponente
            LEFT JOIN public.unidadegestora unc ON unc.ungcod = t.ungcodconcedente
            WHERE
                t.tcpid in (
                    select distinct tc.tcpid from ted.termocompromisso tc
                    left join ted.previsaoorcamentaria po on tc.tcpid = po.tcpid
                    where tc.tcpstatus = 'A'
                )
            %s
        ";

        $filtro = new Ted_Model_Responsabilidade();
        $where = $filtro->getClausleWhere();

        if ($where) {
            $complement = $where;
        } else { $complement='true = true'; }

        $stmt = sprintf($strSQL, ' AND ' . $complement);
        //ver($stmt, d);
        $results = self::dbGetInstance()->carregar($stmt);
        $teds = array();
        if ($results) {
            foreach ($results as $row) {
                $teds[] = $row['tcpid'];
            }
        }

        if (self::possuiPerfil(PERFIL_SUPER_USUARIO)) {
            return true;
        }

        if (self::possuiPerfil(PERFIL_CGSO)) {
            return true;
        }

        if (!in_array(self::capturaTcpid(), $teds)) {
            $urlBase = 'ted.php?modulo=principal/termoexecucaodescentralizada';
            echo self::redirect($urlBase.'/listarTermos', array('acao' => 'A'));
        }
    }

    /**
     * Retorna data da Vigencia, acompanhada da data da assinatura do representante legal do concedente
     * @return bool|string
     */
    public static function pegarVigenciaTED($tcpid = null)
    {
        $tcpid = (is_null($tcpid)) ? self::capturaTcpid() : $tcpid;

        $strSQL = "
          SELECT
                TO_CHAR(dtvigenciaincial, 'dd/mm/yyyy')                 AS datainicio,
                TO_CHAR(dtvigenciafinal, 'dd/mm/yyyy') AS datafim
            FROM
                ted.termocompromisso tcp
            WHERE tcp.tcpid = {$tcpid}
        ";
        $row = self::dbGetInstance()->pegaLinha($strSQL);
        #ver($row, $strSQL, d);
        if (!$row) return false;

        return $row['datainicio'] .' - '. $row['datafim'];
    }

    /**
     * Retorna a data fragmentada no formato array
     * @param $dateUs
     * @return array
     */
    public static function getDateFragment($dateUs)
    {
        if (strlen($dateUs)) {
            return array(
                'yy' => trim(substr($dateUs,0,4)),
                'mm' => trim(substr($dateUs,5,2)),
                'dd' => trim(substr($dateUs,8,2))
            );
        }

        return false;
    }

    /**
     * Checa se o usuário esta lotado para alguma unidade gestora
     * @return bool
     */
    public static function checkUsuarioResponsabilidade()
    {
        if (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO, PERFIL_UG_REPASSADORA, PERFIL_CGSO))) {
            return true;
        }

        $strSQL = sprintf("
            select count(*) as responsabilidade from ted.usuarioresponsabilidade
            where usucpf = '%s' and rpustatus = 'A'
        ", $_SESSION['usucpf']);

        $result = (bool) self::dbGetInstance()->pegaUm($strSQL);
        return ($result) ? $result  : false;
    }

    /**
     * Verifica se possuí perfil para tramitação em lote
     * @return bool
     */
    public static function quemTramitaTermoLote()
    {
        if (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO, PERFIL_UG_REPASSADORA, PERFIL_CGSO))) {
            return true;
        }

        if (possui_perfil(array(
                PERFIL_UG_REPASSADORA, PERFIL_CGSO,
                PERFIL_DIRETORIA_FNDE, PERFIL_SECRETARIO,
                PERFIL_PROREITOR_ADM, PERFIL_SUBSECRETARIO
            ))) {
            return true;
        }

        return false;
    }

    /**
     * Possuí perfil gestor do módulo
     * @return bool
     */
    public static function possuiPerfilGestor()
    {
        if (possui_perfil_gestor(array(PERFIL_SUPER_USUARIO, PERFIL_UG_REPASSADORA, PERFIL_CGSO))) {
            return true;
        }

        return false;
    }
}