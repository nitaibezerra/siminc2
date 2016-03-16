<?php

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once "_constantes.php";
include_once '_funcoes.php';
include_once '_funcoesPar.php';
include_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";
require_once APPRAIZ . "includes/classes/Controle.class.inc";
require_once APPRAIZ . "includes/classes/Visao.class.inc";
require_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once 'autoload.php';
// atualiza ação do usuário no sistema
include APPRAIZ . "includes/registraracesso.php";

class Ajax {

    public $db;

    public function __construct($db = false) {
        if ($db) {
            $this->db = new cls_banco();
        }
    }

    public function equipeTecnica() {
        header('content-type: text/html; charset=ISO-8859-1');

        echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td align=\"left\" colspan=\"2\"><b>Dados 1</b></td>
			</tr>
		</table>";
        die;
    }

    public function analiseMunicipio() {
        header('content-type: text/html; charset=ISO-8859-1');

        echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td align=\"left\" colspan=\"2\"><b>Dados 2</b></td>
			</tr>
		</table>";
        die;
    }

    public function tipoFinanceiras() {
        header('content-type: text/html; charset=ISO-8859-1');

        echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td align=\"left\" colspan=\"2\"><b>Dados 3</b></td>
			</tr>
		</table>";
        die;
    }

    public function termoCompromisso() {
        header('content-type: text/html; charset=ISO-8859-1');

        echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">
			<tr>
				<td align=\"left\" colspan=\"2\"><b>Dados 4</b></td>
			</tr>
		</table>";
        die;
    }

    /**
     * Método que verifica se existe instrumento Unidade e grava na sessão, senão existe ele grava e coloca na sessão
     * @param array $post
     * @return
     */
    public function verificaInstrumentoUnidade($post) {
        $inuid = $post['inuid'];
        if (!$inuid) {
            $obEntidadeParControle = new EntidadeParControle();
            $inuid = $obEntidadeParControle->gravaInstrumentoUnidade($post);
        }
        if ( $inuid && $inuid!='undefined' ) {
        	
        	if( !$this->db ){
        		$this->db = new cls_banco();
        	}
        	
            $sql = "SELECT itrid FROM par.instrumentounidade WHERE inuid = $inuid";
            
            $_SESSION['par']['itrid'] = $this->db->pegaUm($sql);
            if ($post['estuf'] == 'DF') {
                $_SESSION['par']['itrid'] = 1;
            }
            $_SESSION['par']['inuid'] = $inuid;
            $_SESSION['par']['estuf'] = $post['estuf'];
            $_SESSION['par']['muncod'] = $post['muncod'];
        }
        echo $_SESSION['par']['inuid'];
        die;
    }

    /**
     * Método
     * @param array $post
     * @return
     */
    public function guiaSubacao($post) {
        $oConfiguracaoControle = new ConfiguracaoControle();
        $oConfiguracaoControle->filtraGuiaFormSubacao($post['frmid']);
        unset($oConfiguracaoControle);
        die;
    }

    public function montaComboMunicipioPorUf($post) {
        header('content-type: text/html; charset=ISO-8859-1');

        if (!$post['estuf']) {
            die($this->db->monta_combo("muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_', false, null, 'Município'));
        }


        if ($_SESSION['par']['muncod'] && ($_SESSION['par']['itrid'] == 2 || $_SESSION['par']['esfera'] == 'M')) {
            $where = " and muncod = '{$_SESSION['par']['muncod']}' ";
        }

        $sql = "select
				 muncod as codigo, mundescricao as descricao
				from
				 territorios.municipio
				where
				 estuf = '" . $post['estuf'] . "'
				 $where
				order by
				 mundescricao asc";
        die($this->db->monta_combo("muncod_", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'muncod_'));
    }

    public function verificaCepMunicipio($post) {
        $cep = str_replace(array('.', '-'), '', $post['cep']);
        echo $this->db->pegaUm("SELECT muncod FROM cep.v_endereco2 WHERE cep='" . $cep . "' ORDER BY cidade ASC");
        die;
    }

    public function montaArvoreAberta($post = array()) {

        if (!$_SESSION['par']['preid']) {
            echo "<script>alert('Erro na sessão!');history.back(-1);</script>";
            exit();
        }

        $ptoid = $this->verificaTipoObra($_SESSION['par']['preid']);
        $ptoid = $_POST['ptoid'] ? $_POST['ptoid'] : $ptoid;


        $tipoFundacao = $this->verificaTipoFundacao($_SESSION['par']['preid']);

        $where = $where2 = $whereBoFilhos = $sql2 = $boFiltro = "";

        $and = "";
        if ($_POST['_tartarefa']) {
            $where .= " t._tartarefa = {$_POST['_tartarefa']} ";
            $where2 .= " t._tartarefa = {$_POST['_tartarefa']} ";
            $whereBoFilhos .= " and t._tartarefa = {$_POST['_tartarefa']} ";
        }

        $arTipos = array(2, 7);
        if (!empty($tipoFundacao) && in_array($ptoid, $arTipos)) {
            $stWhere .= " AND itctipofundacao = '{$tipoFundacao}' ";
        }

        $sql = "SELECT
					po.docid, po.estuf, pto.ptocategoria
				FROM
					obras.preobra po
				LEFT JOIN obras.pretipoobra pto on pto.ptoid = po.ptoid
				WHERE
				 	po.preid = " . (integer) $_SESSION['par']['preid'];

        $docid = $this->db->pegaLinha($sql);

        if (!empty($docid['docid'])) {
            $sql = "SELECT
					ed.esdid
				FROM
					workflow.documento d
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					d.docid = " . $docid['docid'];

            $esdid = (integer) $this->db->pegaUm($sql);
        }

        $verifica = false;
        $sql = "SELECT 
					pto.ptocategoria
				FROM 
					obras.preobra po
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
				WHERE 
					po.preid = {$_SESSION['par']['preid']}";
        $categoria = $this->db->pegaUm( $sql );

        if ($categoria) {
            $verifica = true;
        }

        if ($verifica || $esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI) {

            include_once(APPRAIZ . "par/classes/WSSigarp.class.inc");
            $oWSSigarp = new WSSigarp();

            $atualizaMI = false;
            if ($_POST['atualmi']) {
                $atualizaMI = true;
            }

            $arrayD = $itemCategoriaObra = $oWSSigarp->listarItemCategoria($docid['ptocategoria'], $docid['estuf'], (integer) $_SESSION['par']['preid'], $atualizaMI);
//			$arrayD = $itemCategoriaObra = $oWSSigarp->solicitarItemObra();

            echo $arrayD;
            die;
        } else {


        	$sql = "SELECT
		        		pto.ptoplanilhaporregiao
		        	FROM
		        		obras.preobra po
		        	INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
		        	WHERE
		        		po.preid = {$_SESSION['par']['preid']}";
        	$ptoplanilhaporregiao = $this->db->pegaUm( $sql );
        	
        	$whereRegiao = " 1=1";
        	if( $ptoplanilhaporregiao == 'E' ){
	        	$whereRegiao = "( (itc.umdid IS NOT NULL AND pir.itcid IS NOT NULL) OR (itc.umdid IS NULL AND pir.itcid IS NULL) )";
        	}
        	
            $sql = "SELECT
						itc.itcid,
						itc.itcdescricao as itcdescricao,
						itc.itcordem,
						itc.itcstatus,
						itc.itcdtinclusao,
						itc.ptoid,
						itc.itcidpai,
						--coalesce(itc.itcvalorunitario::numeric, 0) as itcvalorunitario,
						coalesce(' '||pir.pirvalor, itc.itcvalorunitario) as itcvalorunitario,
						--itcvalorunitario,
						CASE WHEN ptopreencher IS TRUE
							THEN coalesce(ppo.ppovalorunitario, 0)
							ELSE coalesce(coalesce(pir.pirvalor, itc.itcvalorunitario::numeric), coalesce(ppo.ppovalorunitario, 0))
						END as ppovalorunitario,
						pir.pirvalor,
						umd.umdeesc,
						ppo.ppoid,
						pre.preid,
						itc.itctipofundacao,
						itc.itccodigoitem,
						itc.itcidpai,
						itc.itccodigoitemcodigo,
						coalesce(pir.pirqtd, itc.itcquantidade) as itcquantidade,
						pto.ptopreencher
					FROM obras.preobra pre
					INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
					INNER JOIN obras.preitenscomposicao 		itc ON itc.ptoid = pre.ptoid
					LEFT  JOIN obras.preitencomposicao_regiao 	pir ON pir.itcid = itc.itcid AND pir.estuf = pre.estuf
					LEFT  JOIN obras.unidademedida 				umd ON umd.umdid = itc.umdid
					LEFT  JOIN obras.preplanilhaorcamentaria 	ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
					WHERE 
            			pre.ptoid = {$ptoid}
						AND itcstatus = 'A'
						AND pre.preid = '{$_SESSION['par']['preid']}'
						AND itccodigoitemcodigo <> ''
						AND $whereRegiao
						{$stWhere}
					ORDER BY itccodigoitemcodigo
					--limit 1
					";
// 					ver($sql,d);
            $arDados = $this->db->carregar($sql);

            $sql = "select sum(total) from (
						SELECT
							--trim(to_char(coalesce(sum(itc.itcvalorunitario::numeric*ppo.ppovalorunitario), 0), '9999999990D99')) as total
							itc.itcvalorunitario::numeric*ppo.ppovalorunitario as total
						FROM obras.preobra pre
							INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
							LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
							LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
						WHERE pre.ptoid = {$ptoid}
						AND pre.preid = '{$_SESSION['par']['preid']}'
						AND itcstatus = 'A'
						{$stWhere}
					) as total
					";

            $total = $this->db->pegaUm($sql);

            $count = 0;
            $count2 = 0;
        }

        if (is_array($arDados) && $arDados[0] != "") {
            $itens = array();
            $boCima = $boBaixo = false;
            foreach ($arDados as $dados) {
                //$img = $dados["taraberto"] == 't' ?  "menos.gif" : "mais.gif";
                $img = "menos.gif";
                /**
                 * Verifica se tem Filho
                 */
                $sql = "SELECT
							count(1)
						FROM obras.preobra pre
						INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
						WHERE pre.ptoid = {$ptoid}
							AND pre.preid = '{$_SESSION['par']['preid']}'
							AND itc.itcstatus = 'A'
							AND itc.itccodigoitemcodigo <> ''
							AND itc.itcidpai = {$dados['itcid']}";

                $boFilho = $this->db->pegaUm($sql);

                $nivelCorrente = (strlen($dados['itccodigoitemcodigo']) / 3);

                $arDados[$count]['boFilho'] 			= $boFilho;
                $arDados[$count]['img'] 				= $img;
                // se nao tiver quantidade, retorna 0. se tiver e for inteiro, retorna o inteiro (sem casas decimais). se tiver casas decimais, formato com duas casas e o divisor é ponto para manter compatibilidade
                $arDados[$count]['itcquantidade'] 		= !$dados['itcquantidade'] ? 0 : ((is_int($dados['itcquantidade'])) ? $dados['itcquantidade'] : number_format($dados['itcquantidade'], 2, ".", ""));
                //$arDados[$count]['itccodigoitemcodigo'] = !$dados['itccodigoitemcodigo'] ? "0" : (int)$dados['itccodigoitemcodigo'];
                $arDados[$count]['itccodigoitemcodigo'] = $dados['itccodigoitemcodigo'];
                $arDados[$count]['itcdescricao'] 		= utf8_encode($dados['itcdescricao']);
                $arDados[$count]['umdeesc'] 			= ucfirst($dados['umdeesc']);
                $arDados[$count]['ppovalorunitario'] 	= ucfirst($dados['ppovalorunitario']);
                $arDados[$count]['total'] 				= $total;
                $arDados[$count]['ptopreencher'] 		= $dados['ptopreencher'];

                $count++;
            }
            echo simec_json_encode($arDados);
        } else {
            echo '';
        }
        die;
    }

    /**
     * FUNÇÃO PARA FORMAR CÓDIGO DA TAREFA
     *
     * @param string $_tarordem
     * @param array $arCodTarefa
     * @return $arCodTarefa
     */
    private function formaCodTarefa($tarordem, &$arCodTarefa) {
        if (strlen($tarordem) < 4) {
            $blocoCod = substr($tarordem, 0, 3);
            return $arCodTarefa[] = intval($blocoCod);
        }

        $blocoCod = substr($tarordem, 0, 3);
        $arCodTarefa[] = intval($blocoCod);
        return self::formaCodTarefa(substr($tarordem, 3), $arCodTarefa);
    }

    public function verificaGrupoMunicipioTipoObra_A($dados) {

        $sql = "SELECT muncod FROM territorios.muntipomunicipio  WHERE tpmid = " . TIPOMUN_GRUPO1 . " AND muncod='" . $_SESSION['par']['muncod'] . "'";
        $muncodGrupo = $this->db->pegaUm($sql);
        if ($muncodGrupo)
            echo "true";
        else
            echo "false";
    }

    public function verificaTipoObra($preid) {
        $sql = "SELECT
					ptoid
				FROM obras.preobra pre
				WHERE preid = '{$preid}'";

        return $preid ? $this->db->pegaUm($sql) : false;
    }

    public function verificaTipoFundacao($preid) {
        $sql = "SELECT
					pretipofundacao
				FROM obras.preobra pre
				WHERE preid = '{$preid}'";

        return $this->db->pegaUm($sql);
    }

    public function filtraTipoSubAcao() {
        $db = new cls_banco();
        $frmid = $_POST['frmid'];
        $ptsid = $_POST['ptsid'];
        $sql = "SELECT
					ptsid as codigo,
					tps.tpsdescricao as descricao
				FROM
					par.propostatiposubacao pts
				INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
				WHERE
					frmid = $frmid
				AND
					ptsstatus = 'A'
				ORDER BY
					descricao";
        $arrDados = $db->carregar($sql);
        if ($arrDados) {
            $db->monta_combo('ptsid', $arrDados, 'S', 'Selecione...', 'carregaAbasSubacao', '', '', '', 'S', '', '', $ptsid);
        } else {
            return false;
        }
    }

    public function carregaAbasSubacao() {

        if ($_POST['ppsid']) {
            $aba = "par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A&acaoGuia=editar&ppsid={$_POST['ppsid']}";
        } else {
            $aba = "par.php?modulo=principal/configuracao/popupGuiaSubacao&acao=A&acaoGuia=incluir&indid={$_POST['indid']}";
        }
        print "<br />";
        print carregaAbasPropostaSubacao($aba, $_POST['frmid'], $_POST['ppsid'], $_POST['indid'], $_POST['ptsid']);
    }

    public function questionarioProfuncionario() {
        $db = new cls_banco();

        $sql = "select
                    itptitulo
                from questionario.resposta res
                inner join questionario.itempergunta itp on itp.itpid = res.itpid
                where res.qrpid = {$_POST['qrpid']}
                and itp.itpid = " . RESPOSTA1_BLOQUEIA_PROFUN . "
                order by res.resid desc";

        echo $db->pegaUm($sql);
    }

    public function verificaTipoEscola($post) {
        $db = new cls_banco();

        if ($post['ptoid'] != '') {
            $sql = "SELECT DISTINCT
						'true'
					FROM
						obras.pretipoobra
					WHERE
						ptoexisteescola = TRUE 
						AND ptoid = " . $post['ptoid'];

            echo $db->pegaUm($sql);
        }
    }
}

if (isset($_POST['requisicao'])) {
    $db = ( isset($_POST['db']) && !empty($_POST['db']) ) ? true : false;
    $obAjax = new Ajax($db);
    $obAjax->$_POST['requisicao']($_POST);
}
?>