<?php

class Empreendimento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.empreendimento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("empid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'empid' => null,
        'orgid' => null,
        'empesfera' => null,
        'tpoid' => null,
        'prfid' => null,
        'moeid' => null,
        'tobid' => null,
        'tooid' => null,
        'cloid' => null,
        'endid' => null,
        'demid' => null,
        'preid' => null,
        'entidunidade' => null,
        'entidcampus' => null,
        'docid' => null,
        'arqid' => null,
        'empdsc' => null,
        'empcomposicao' => null,
        'empjustsitdominial' => null,
        'empvalorprevisto' => null,
        'empdtultvistoria' => null,
        'emppercentultvistoria' => null,
        'empdtultvistoriaempresa' => null,
        'emppercentultvistoriaempresa' => null,
        'empdtultvistoriami' => null,
        'emppercentultvistoriami' => null,
        'empdtinclusao' => null,
        'empdtcarga' => null,
        'empstatus' => null,
        'obrid_1' => null,
        'empdtultvistoriafnde' => null,
        'emppercentultvistoriafnde' => null,
    );

    public function excluirImagem($empid, $arqid) {
        $sql = "UPDATE obras2.empreendimento 
	    		SET arqid=NULL 
	    		WHERE empid='{$empid}'";
        $this->executar($sql);

        // Tive que tirar a exclusão do arquivo devido as replicas da obra	    
//	    include_once APPRAIZ."includes/classes/fileSimec.class.inc";
//	    
//	    $file       = new FilesSimec("empreendimento", array(true), "obras2");
//	    $file->setPasta('obras2');
//	    $file->setPulaTableEschema( true );
//	    $file->setRemoveUpload($arqid);
    }

    public function listaDados(Array $param = array()) {
        $arWhere = array();

        if (!empty($param['mescod'])) {
            $param['mescod'] = (array) $param['mescod'];
            $arWhere[] = "m.mescod IN('" . implode("', '", $param['mescod']) . "')";
        }

        if (!empty($param['orgid'])) {
            $param['orgid'] = (array) $param['orgid'];
            $arWhere[] = "e.orgid IN(" . implode(", ", $param['orgid']) . ")";
        }

        $sql = "SELECT
					e.*,
					mr.mesdsc,
					mi.micdsc,
					m.estuf,
					m.mundescricao,
					o.obrid,
					o.obrnome
				FROM
					obras2.empreendimento e
				JOIN entidade.endereco ed ON ed.endid = e.endid AND
											 ed.endstatus = 'A'
				JOIN territorios.municipio m ON m.muncod = ed.muncod 
				JOIN territorios.mesoregiao mr ON mr.mescod = m.mescod
				JOIN territorios.microregiao mi ON mi.miccod = m.miccod
				JOIN obras2.obras o ON o.empid = e.empid AND o.obridpai IS NULL and o.obrstatus = 'A'
				WHERE
					e.empstatus = 'A'" .
                (count($arWhere) ? " AND " . implode(' AND ', $arWhere) : "");
        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }

    public function listaDadosPorEmpid($empids) {
        $arWhere = array();

        $sql = "SELECT
					e.*,
					mr.mesdsc,
					mi.micdsc,
					m.estuf,
					m.mundescricao,
                    m.mescod,
                    o.obrid,
                    o.obrnome
				FROM
					obras2.empreendimento e
				JOIN entidade.endereco ed ON ed.endid = e.endid AND
											 ed.endstatus = 'A'
				JOIN territorios.municipio m ON m.muncod = ed.muncod 
				JOIN territorios.mesoregiao mr ON mr.mescod = m.mescod
				JOIN territorios.microregiao mi ON mi.miccod = m.miccod
				JOIN obras2.obras o ON o.empid = e.empid AND o.obridpai IS NULL and o.obrstatus = 'A'
				WHERE
					e.empstatus = 'A'
                    AND e.empid IN(" . implode(',', $empids) . ")";
        $dados = $this->carregar($sql);
        return ($dados ? $dados : array());
    }

    public function exibirListaObraPopUp() {
        $arrDados = $_REQUEST;
        extract($arrDados);

        $osObra = new Supervisao_Os_Obra();
        $arEmpid = $osObra->listaEmpidPorOs($arrDados['sosid']);
        ?>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <script>
            function addObra(empid)
            {
                if ($("#chk_empid_" + empid).attr("checked") == true) {
                    window.opener.addObraOS(empid);
                } else {
                    window.opener.removeObraOS(empid);
                }
            }
            function PesquisarObra()
            {
                $("#formulario_pesquisa").submit();
            }
            function LimparObra()
            {
                window.location.href = window.location;
            }
        </script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        <?php
        monta_titulo('Selecione a(s) obra(s)', '');
        ?>
        <form id="formulario_pesquisa" name="formulario_pesquisa" method="post" action="">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                <tr>
                    <td width="25%" class="SubTituloDireita">ID / Nome da Obra</td>
                    <td><?= campo_texto('empdsc', 'N', "S", '', 40, 255, '', '', 'left', '', 0, 'id="empdsc"', '', $empdsc); ?></td>
                </tr>
        <?php
        montaComboTerritorio("emp", array("estuf" => array("obrigatorio" => "N", "value" => $_POST['empestuf']), "mescod" => array("obrigatorio" => "N", "disabled" => "S", "value" => $_POST['empmescod']), "miccod" => array("disabled" => "S", "value" => $_POST['empmiccod']), "muncod" => array("disabled" => "S", "value" => $_POST['empmuncod']), 'sgrid' => $arrDados['sgrid']));
        ?>

                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
                    <td>
                        <?php
                        $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                        $this->monta_combo('esdid', $sql, "S", "Todos", "", "", "", "200", "N", "esdid", "", $esdid);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" style="width: 190px;">OS em andamento:</td>
                    <td>
                        <input type="radio" name="osandamento" id="" value="S" <?= ( $_POST["osandamento"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                        <input type="radio" name="osandamento" id="" value="N" <?= ( $_POST["osandamento"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                        <input type="radio" name="osandamento" id="" value=""  <?= ( $_POST["osandamento"] == "" ? "checked='checked'" : "" ) ?>/> Todas
                    </td>
                </tr>
                <tr>
                    <td colspan="2" class="SubTituloCentro">
                        <input type="button" name="btn_pesquisar" value="Pesquisar" onclick="PesquisarObra()" />
                        <input type="button" name="btn_pesquisar" value="Ver Todas" onclick="LimparObra()" />
                    </td>
                </tr>

            </table>
        </form>
        <?php
        if (!empty($arrDados['mescod']) && $arrDados['mescod'] != "undefined") {
            $arrMescod = explode(",", $arrDados['mescod']);
            $arWhere[] = "m.mescod IN('" . implode("','", $arrMescod) . "')";
        }

        if (!empty($arrDados['orgid']) && $arrDados['orgid'] != "undefined") {
            $arWhere[] = "e.orgid IN(" . $arrDados['orgid'] . ")";
        }

        if (!empty($arrDados['empdsc'])) {
            $empdscTmp = removeAcentos(str_replace("-"," ",$arrDados['empdsc']));
            $arWhere[] = " UPPER( public.removeacento(o.obrnome)) ilike ('%" . $empdscTmp . "%')
                           OR o.obrid::text ilike('%{$arrDados['empdsc']}%')
                           OR UPPER( public.removeacento(e.empdsc)) ilike ('%" . $empdscTmp . "%')
                           OR o.empid::text like('%{$arrDados['empdsc']}%')";
        }

        if ($arrDados['empestuf']) {
            $arrDados['empestuf'] = (array) $arrDados['empestuf'];
            $arWhere[] = "tes.estuf IN('" . implode("', '", $arrDados['empestuf']) . "')";
        }
        if ($arrDados['empmescod']) {
            $arWhere[] = " mr.mescod = '" . $arrDados['empmescod'] . "'";
        }
        if ($arrDados['empmiccod']) {
            $arWhere[] = " mi.miccod = '" . $arrDados['empmiccod'] . "'";
        }
        if ($arrDados['empmuncod']) {
            $arWhere[] = " m.muncod = '" . $arrDados['empmuncod'] . "'";
        }
        if ($arrDados['esdid']) {
            $arWhere[] = " esd.esdid = '" . $arrDados['esdid'] . "'";
        }
        $sql = "SELECT
                    e.empid,
                    o.obrid,
                    o.obrnome as obra,
                    m.estuf,
                    mr.mesdsc,
                    mi.micdsc,
                    m.mundescricao,
                    CASE
                        WHEN esd.esdid = " . ESDID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || esd.esddsc || '</font>'
                        WHEN esd.esdid = " . ESDID_OBJ_CANCELADO . " THEN '<font COLOR=\"#000000\" style=\"font-weight:bold\">' || esd.esddsc || '</font>'
                        ELSE esd.esddsc
                    END as situacao,

                    /*
                        Criada para segurança, mas um empreendimento só pode ter uma obra
                        array_to_string(
                        array(
                                SELECT
                                    CASE
                                        WHEN ed.esdid = " . ESDID_OBJ_CONCLUIDO . " THEN '<font COLOR=\"#0066CC\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                                        WHEN ed.esdid = " . ESDID_OBJ_CANCELADO . " THEN '<font COLOR=\"#000000\" style=\"font-weight:bold\">' || ed.esddsc || '</font>'
                                        ELSE ed.esddsc
                                    END as situacao
                                FROM obras2.obras o
                                LEFT JOIN workflow.documento d ON d.docid = o.docid AND tpdid = 105
                                LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
                                WHERE o.empid = e.empid
                    ), ', ') as situacao,*/

                    (SELECT count(*) FROM obras2.supervisao_os os
                    JOIN obras2.supervisao_os_obra osobr ON osobr.sosid = os.sosid AND osobr.soostatus = 'A'
                    LEFT JOIN workflow.documento d ON d.docid = os.docid AND tpdid = 115
                    WHERE osobr.empid = e.empid
                    AND os.sosstatus = 'A'
                    AND d.esdid NOT IN (" . ESDID_OS_CANCELADA . ", " . ESDID_OS_CONCLUIDA . ")) as qtdos,

                    (SELECT count(*) FROM obras2.supervisao_os os
                    JOIN obras2.supervisao_os_obra osobr ON osobr.sosid = os.sosid AND osobr.soostatus = 'A'
                    LEFT JOIN workflow.documento d ON d.docid = os.docid AND tpdid = 115
                    WHERE osobr.empid = e.empid
                    AND os.sosstatus = 'A'
                    AND d.esdid NOT IN (" . ESDID_OS_CANCELADA . ")) as qtdnaocancelada,
                    CASE WHEN o.tpoid IN (104,105) THEN
                        '<img title=\"Aditivos da Obra\" style=\"cursor:pointer;width:15px;\" src=\"/imagens/obras/check.png\">'
                    ELSE
                        ''
                    END as mi,

                    (SELECT
                        TO_CHAR(se.suedtsupervisao, 'dd/mm/yyyy')
                    FROM
                        obras2.empreendimento e
                    JOIN obras2.obras                            o1 ON o.empid    = e.empid AND o1.obrstatus = 'A' AND o1.obridpai IS NULL
                    JOIN obras2.supervisao_os_obra              oo ON oo.empid   = e.empid  AND oo.soostatus = 'A'
                    JOIN obras2.supervisao_os                   os ON os.sosid   = oo.sosid AND os.sosstatus = 'A'
                    JOIN workflow.documento                      d ON d.docid    = os.docid	AND d.esdid <> " . ESDID_OS_CANCELADA . "
                    JOIN obras2.supervisaoempresa          se ON se.suestatus = 'A' AND se.empid = e.empid and  se.sosid   = os.sosid
                    JOIN workflow.documento                d1 ON d1.docid   = se.docid AND d1.esdid <> " . WF_ESDID_LAUDO_SUPERVISAO_CANCELADO . "
                    WHERE e.empstatus = 'A' AND e.orgid IN(3) AND o.obrid = o1.obrid
                    ORDER BY 1 DESC
                    LIMIT 1) as ultimasup
            FROM
                    obras2.empreendimento e
            JOIN entidade.endereco ed ON ed.endid = e.endid AND
                                                                     ed.endstatus = 'A'
            JOIN territorios.estado tes ON tes.estuf = ed.estuf											 
            JOIN territorios.municipio m ON m.muncod = ed.muncod 
            JOIN territorios.mesoregiao mr ON mr.mescod = m.mescod
            JOIN territorios.microregiao mi ON mi.miccod = m.miccod
            JOIN obras2.obras o ON o.empid = e.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
            LEFT JOIN workflow.documento d ON d.docid = o.docid
            LEFT JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
            WHERE
                    e.empidpai IS NULL AND e.empstatus = 'A'" .
            (count($arWhere) ? " AND " . implode(' AND ', $arWhere) : "");


        if ($arrDados['osandamento'] == 'S') {
            $sql = "SELECT foo.* FROM ($sql) as foo WHERE foo.qtdos > 0";
        }
        if ($arrDados['osandamento'] == 'N') {
            $sql = "SELECT foo.* FROM ($sql) as foo WHERE foo.qtdos < 1";
        }

        $arrDados = $this->carregar($sql);
        $arrCab = array("Ação", "ID Obra", "Obra", "UF", "Mesorregião", "Microrregião", "Município", "Situação", "OS em andamento", "Quantidade de OS", "Última Sup.", "MI");
        $arrDados = $arrDados ? $arrDados : array();

        foreach ($arrDados as $n => $dado) {
            if (in_array($dado['empid'], $arEmpid)) {
                $checkbox = "<input checked=\"checked\" type=\"checkbox\" id=\"chk_empid_{$dado['empid']}\" name=\"chk_empid[]\" value=\"{$dado['empid']}\" onclick=\"addObra('{$dado['empid']}')\" />";
            } else {
                $checkbox = "<input type=\"checkbox\" id=\"chk_empid_{$dado['empid']}\" name=\"chk_empid[]\" value=\"{$dado['empid']}\" onclick=\"addObra('{$dado['empid']}')\" />";
            }
            $arrResultado[$n]['acao'] = $checkbox;
            $arrResultado[$n]['obrid'] = $dado['obrid'];
            $arrResultado[$n]['obra'] = $dado['obra'];
            $arrResultado[$n]['estuf'] = $dado['estuf'];
            $arrResultado[$n]['mesdsc'] = $dado['mesdsc'];
            $arrResultado[$n]['micdsc'] = $dado['micdsc'];
            $arrResultado[$n]['mundescricao'] = $dado['mundescricao'];
            $arrResultado[$n]['situacao'] = $dado['situacao'];
            $arrResultado[$n]['qtdos'] = ($dado['qtdos'] > 0) ? "Sim" : "Não";
            $arrResultado[$n]['qtdnaocancelada'] = $dado['qtdnaocancelada'];
            $arrResultado[$n]['ultimasup'] = $dado['ultimasup'];
            $arrResultado[$n]['mi'] = $dado['mi'];
        }
        $arrResultado = $arrResultado ? $arrResultado : array();

        $this->monta_lista_array($arrResultado, $arrCab, 100, 10, "S", "center");
        echo "<center><input type=\"button\" value=\"Fechar\" name=\"btn_fechar\" onclick=\"window.close()\" /></center>";
        die;
    }

    public function listaEmpreendimento(Array $param = array()) {
        $where = array();
        $join = array();

        $param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);

        if ($param['orgid']) {
            $param['orgid'] = (array) $param['orgid'];
            $where[] = "vo.orgid IN(" . implode(", ", $param['orgid']) . ")";
        }

        if ($param['empbuscatexto']) {
            $empbuscatextoTemp = removeAcentos(str_replace("-"," ",(trim($param['empbuscatexto']) )));
            $where[] = " ( UPPER(public.removeacento(vo.empdsc)) ILIKE ('%" . $empbuscatextoTemp . "%') OR
			   public.removeacento(vo.empid::CHARACTER VARYING) ILIKE ('%" . $empbuscatextoTemp . "%') ) ";
        }

        if ($param['tobid']) {
            $param['tobid'] = (array) $param['tobid'];
            $where[] = "vo.tobid IN(" . implode(", ", $param['tobid']) . ")";
        }

        if ($param['cloid']) {
            $param['cloid'] = (array) $param['cloid'];
            $where[] = "vo.cloid IN(" . implode(", ", $param['cloid']) . ")";
        }

        if ($param['tpoid']) {
            $param['tpoid'] = (array) $param['tpoid'];
            $where[] = "vo.tpoid IN(" . implode(", ", $param['tpoid']) . ")";
        }

        if (!empty($param['IS(mi)'])) {
            $where[] = "vo.tpoid IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if (!empty($param['NOT(mi)'])) {
            $where[] = "vo.tpoid NOT IN (" . TPOID_MI_TIPO_B . ", " . TPOID_MI_TIPO_C . ")";
        }

        if ($param['prfid']) {
            $param['prfid'] = (array) $param['prfid'];
            $where[] = "vo.prfid IN(" . implode(", ", $param['prfid']) . ")";
        }

        if ($param['moeid']) {
            $param['moeid'] = (array) $param['moeid'];
            $where[] = "vo.moeid IN(" . implode(", ", $param['moeid']) . ")";
        }

        if ($param['estuf']) {
            $param['estuf'] = (array) $param['estuf'];
            $where[] = "vo.estuf IN('" . implode("', '", $param['estuf']) . "')";
//			$join['endereco'] = "JOIN entidade.endereco ede ON ede.endid = endid AND ede.endstatus = 'A'";
        }

        if ($param['muncod']) {
            $param['muncod'] = (array) $param['muncod'];
            $where[] = "vo.muncod IN('" . implode("', '", $param['muncod']) . "')";
//			$join['endereco'] = "JOIN entidade.endereco ede ON ede.endid = endid AND ede.endstatus = 'A'";
        }

        if ($param['empesfera']) {
            $param['empesfera'] = (array) $param['empesfera'];
            $where[] = "vo.empesfera IN('" . implode("', '", $param['empesfera']) . "')";
        }

        if ($param['empvalorprevisto_menor']) {
//			$where[] = "empvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_menor']);
            $where[] = "vo.somavlrcontrato < " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_menor']);
        }

        if ($param['empvalorprevisto_maior']) {
//			$where[] = "empvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_maior']);
            $where[] = "vo.somavlrcontrato > " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_maior']);
        }

        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {

//			$arrEst = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_ESTADUAL,
//							PFLCOD_EMPRESA_CONTRATADA);
//			$arrObr = Array(PFLCOD_EMPRESA_CONTRATADA,
//							PFLCOD_SUPERVISOR_UNIDADE);
//			$arrOrg = Array(PFLCOD_ADMINISTRADOR,
//							PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_ESTADUAL,
//							PFLCOD_CONSULTA_TIPO_DE_ENSINO,
//							PFLCOD_SUPERVISOR_MEC);
//			$arrUni = Array(PFLCOD_AUDITOR_INTERNO,
//							PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_UNIDADE,
//							PFLCOD_GESTOR_UNIDADE,
//							PFLCOD_SUPERVISOR_UNIDADE);

            $arrObr = Array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                    /* PFLCOD_SUPERVISOR_UNIDADE */                    );

//			$arrUni = Array(PFLCOD_SUPERVISOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_ESTADUAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

            $resp = array();
            $arPflcod = array();

            if (possui_perfil($arrEst)) {
                $resp[] = "urs.estuf = vo.estuf ";
                $arPflcod = array_merge($arPflcod, $arrEst);
            }

            if (possui_perfil($arrObr)) {
                $resp[] = "urs.empid = vo.empid ";
                $arPflcod = array_merge($arPflcod, $arrObr);
            }
            if (possui_perfil($arrOrg)) {
                $resp[] = "urs.orgid = vo.orgid ";
                $arPflcod = array_merge($arPflcod, $arrOrg);
            }
//			if(possui_perfil($arrUni)){
//				$resp[] = "urs.entid = vo.entidunidade ";
//				$arPflcod = array_merge($arPflcod, $arrUni);
//			}

            if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(0));

                $resp[] = " urs.entid = vo.entidunidade AND vo.empid IN(" . implode(", ", $arEmpid) . ")";
                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
                $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;
            }

            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs ON urs.rpustatus = 'A' AND
                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND 
                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                    (" . implode(' OR ', $resp) . ")";
            }
        }

        $imgExc = '';
        if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_MEC, PFLCOD_GESTOR_MEC))) {
            /*
             * Adonias falou para deixar liberado, por enquanto, para o FNDE poder cadastrar um novo obj vinculado
             * quando a obra estiver paralisada, sem lever em consideração o subtipo (contrato cancelado e recindido)
             */
            $imgExc = "<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirEmp(\'' || vo.empid || '\');\"
	 					title=\"Excluir Obra\">";
        }

        $acao = "'<center><div style=\"width:60px;\">
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: ' || ( CASE WHEN vo.qtdobjetosativos = 1 THEN 'alterarObr(''' || vo.obridativo || ''');' ELSE 'alterarEmp(''' || vo.empid || ''');' END ) || '\"
	 					title=\"Alterar Obra\">
					{$imgExc}
				    <img
	 					align=\"absmiddle\"
	 					src=\"/imagens/principal.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: resumoEmp(\'' || vo.empid || '\');\"
	 					title=\"Resumo da obra\">' || 
				  '</div></center>'";
/*/* 		$A = '\'<img title="Anexos da Obra" style="cursor:pointer; width:15px;" onclick="window.location.href = \'\'obras2.php?modulo=principal/listaObrasEmpreendimento&acao=A&empid=\'|| empid ||\'\'\';" src="/imagens/obras/anexo.png">\'';
// 		$F = '\'<img title="Fotos da Obra" style="cursor:pointer; width:15px;" onclick="window.location.href = \'\'obras2.php?modulo=principal/listaObrasEmpreendimento&acao=A&empid=\'|| empid ||\'\'\';" src="/imagens/cam_foto.gif">\'';
// 		$R = '\'<img title="Restrições \'|| CASE WHEN res_estado = 1 THEN \'Superadas\' WHEN res_estado = 2 THEN \'Pendentes\' ELSE \'em Atrazo\' END || \'" style="cursor:pointer; width:15px;"
// 					onclick="window.location.href = \'\'obras2.php?modulo=principal/listaRestricao&acao=O&empid=\'|| empid ||\'\'\';"
// 					src="/imagens/obras/atencao\'|| CASE WHEN res_estado = 1 THEN \'_Verde\' WHEN res_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';
// 		$I = '\'<img title="Impedimentos \'|| CASE WHEN inc_estado = 1 THEN \'Superadas\' WHEN inc_estado = 2 THEN \'Pendentes\' ELSE \'em Atrazo\' END || \'" style="cursor:pointer; width:15px;"
// 					onclick="window.location.href = \'\'obras2.php?modulo=principal/listaRestricao&acao=O&empid=\'|| empid ||\'\'\';"
// 					src="/imagens/obras/atencao\'|| CASE WHEN inc_estado = 1 THEN \'_verde\' WHEN inc_estado = 2 THEN \'\' ELSE \'_vermelho\' END || \'.png">\'';
        /* BACKUP LEFT FOTOS DOSUCMENTO E RESTRIÇÔES VIEW
         * 
          LEFT JOIN ( SELECT true as tem_documento, empid
          FROM obras2.obras o
          JOIN obras2.obras_arquivos a ON a.obrid = o.obrid AND a.oarstatus = 'A'::bpchar
          WHERE o.obrstatus = 'A'::bpchar AND a.oartipo = 'D'
          GROUP BY o.empid) doc ON doc.empid = e.empid
          LEFT JOIN ( SELECT true as tem_foto, empid
          FROM obras2.obras o
          JOIN obras2.obras_arquivos a ON a.obrid = o.obrid AND a.oarstatus = 'A'::bpchar
          WHERE o.obrstatus = 'A'::bpchar AND a.oartipo = 'F'
          GROUP BY o.empid) fot ON fot.empid = e.empid
          LEFT JOIN (
          SELECT
          max( res_estado ) as res_estado,
          empid
          FROM
          (
          SELECT DISTINCT
          CASE
          WHEN rstdtsuperacao IS NOT NULL THEN 1
          WHEN rstdtprevisaoregularizacao > now() THEN 2
          WHEN rstdtprevisaoregularizacao < now() THEN 3
          ELSE 2
          END as res_estado,
          empid
          FROM
          obras2.restricao
          WHERE
          empid IS NOT NULL
          AND rstitem = 'R'
          ) as  foo
          GROUP BY
          empid
          ) res ON res.empid = e.empid
          LEFT JOIN (
          SELECT
          max( inc_estado ) as inc_estado,
          empid
          FROM
          (
          SELECT DISTINCT
          CASE
          WHEN rstdtsuperacao IS NOT NULL THEN 1
          WHEN rstdtprevisaoregularizacao > now() THEN 2
          WHEN rstdtprevisaoregularizacao < now() THEN 3
          ELSE 2
          END as inc_estado,
          empid
          FROM
          obras2.restricao
          WHERE
          empid IS NOT NULL
          AND rstitem = 'I'
          ) as  foo
          GROUP BY
          empid
          ) inc ON inc.empid = e.empid
         * */
                                        
        if (is_numeric($param['empbuscatexto'])){
            $str_where_vinculos = " AND ( ( oo.obrid::CHARACTER VARYING ILIKE UPPER('90%') AND oo.obrid::CHARACTER VARYING ILIKE UPPER('%".$param['empbuscatexto']."' ) ) 
                                         OR oo.empid = vo.empid )";
//                                         OR oo.empid = ".$param['empbuscatexto']." )";
            
//            $str_where_vinculos = " AND oo.empid = ".$param['empbuscatexto'];
        }
        
        $sql = "SELECT
                        DISTINCT
                        {$acao} AS acao,
                        --Solucao para atender a solicitacao de exibicao de todos os vinculos de um empreendimento
                        oo.obrid,                            
                        --CASE WHEN tem_documento THEN $A ELSE '' END as a,
                        --CASE WHEN tem_foto THEN $F ELSE '' END as f,
                        --CASE WHEN res_estado IS NOT NULL THEN $R ELSE '' END as r,
                        --CASE WHEN inc_estado IS NOT NULL THEN $I ELSE '' END as i,
                        vo.empid,
                        '<a href=\"javascript: ' || ( CASE WHEN vo.qtdobjetosativos = 1 THEN 'alterarObr(''' || vo.obridativo || ''');' ELSE 'alterarEmp(''' || vo.empid || ''');' END ) || '\">' || empdsc || '</a>',
                        COALESCE(vo.entnomeunidade, ' - ') AS entunidade,
                        --COALESCE(vo.entnomecampus, ' - ') AS entcampus,
                        vo.estuf || ' - ' || vo.mundescricao AS uf,
                        TO_CHAR(vo.obrdtinicio,'DD/MM/YYYY') AS obrdtinicio,
                        TO_CHAR(vo.obrdtfim,'DD/MM/YYYY') AS obrdtfim,
                        vo.tpodsc,
                        -- esddsc,
                        vo.somavlrcontrato,
                        '<font ' ||
                        CASE WHEN vo.diasultimavistoria <= 45 THEN
                                        CASE WHEN vo.emppercentultvistoria >= 100.00 THEN
                                                'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                        ELSE
                                                'COLOR=\"#00AA00\" TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                        END
                                 WHEN vo.diasultimavistoria > 45 AND diasultimavistoria <= 60 THEN
                                        CASE WHEN vo.emppercentultvistoria >= 100.00 THEN
                                                'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                        ELSE
                                                'COLOR=\"#BB9900\" TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                        END
                                 WHEN vo.diasultimavistoria > 60 THEN
                                        CASE WHEN vo.emppercentultvistoria >= 100.00 THEN
                                                'COLOR=\"#0066CC\" TITLE=\"Esta obra foi atualizada a mais de 60 dias\">' 
                                        ELSE
                                                'COLOR=\"#DD0000\" TITLE=\"Esta obra está desatualizada\">' 
                                        END
                        END
                        || to_char(vo.empdtultvistoria, 'DD/MM/YYYY')||'</br>( '|| vo.diasultimavistoria ||' dia(s) ) </font>' AS dataUltVistoria,
                        vo.emppercentultvistoria,
                        TO_CHAR(vo.empdtultvistoriaempresa, 'DD/MM/YYYY') AS empdtultvistoriaempresa,
                        vo.emppercentultvistoriaempresa
                        
                FROM
                        obras2.v_obra vo
                " . ( $join ? implode(" ", $join) : "" ) . " 
                
                --Solucao para atender a solicitacao de exibicao de todos os vinculos de um empreendimento
                INNER JOIN obras2.obras oo ON oo.empid = vo.empid

                WHERE
                        " . (count($where) ? " " . implode(' AND ', $where) : "") . "
                            
                ".$str_where_vinculos." 
                    
                
                ORDER BY
                        3";
        
        //ver(simec_htmlentities($sql));
        
        return $sql;
    }

    public function listaEmpreendimentoOS(Array $param = array()) {
        $where = array();
        $join = array();

        $param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);

        if ($param['entid']) {
            $where[] = "ent3.entid = {$param['entid']} ";
        }

        if ($param['obrami'] == 'S') {
            $where[] = "o.tpoid IN (104, 105)";
        }

        if ($param['obrami'] == 'N') {
            $where[] = "o.tpoid NOT IN (104, 105)";
        }

        if ($param['sosterreno'] == 't') {
            $where[] = "os.sosterreno = 't'";
        }

        if ($param['sosterreno'] == 'f') {
            $where[] = "os.sosterreno = 'f'";
        }

        if ($param['orgid']) {
            $param['orgid'] = (array) $param['orgid'];
            $where[] = "e.orgid IN(" . implode(", ", $param['orgid']) . ")";
        }

        if ($param['empbuscatexto']) {
            $empbuscaTextoTmp = removeAcentos($param['empbuscatexto']);
            $where[] = "( UPPER(public.removeacento(o.obrnome)) ILIKE UPPER('%" . $empbuscaTextoTmp . "%') OR
						   o.obrid::CHARACTER VARYING ILIKE UPPER('%" . $param['empbuscatexto'] . "%') ) ";
        }

        if ($param['sosnum']) {
            $where[] = " os.sosnum::text like('%" . $param['sosnum'] . "%')";
        }

        if ($param['tobid']) {
            $param['tobid'] = (array) $param['tobid'];
            $where[] = "e.tobid IN(" . implode(", ", $param['tobid']) . ")";
        }

        if ($param['cloid']) {
            $param['cloid'] = (array) $param['cloid'];
            $where[] = "e.cloid IN(" . implode(", ", $param['cloid']) . ")";
        }

        if ($param['tpoid']) {
            $param['tpoid'] = (array) $param['tpoid'];
            $where[] = "e.tpoid IN(" . implode(", ", $param['tpoid']) . ")";
        }

        if ($param['prfid']) {
            $param['prfid'] = (array) $param['prfid'];
            $where[] = "e.prfid IN(" . implode(", ", $param['prfid']) . ")";
        }

        if ($param['moeid']) {
            $param['moeid'] = (array) $param['moeid'];
            $where[] = "e.moeid IN(" . implode(", ", $param['moeid']) . ")";
        }

        if ($param['estuf']) {
            $param['estuf'] = (array) $param['estuf'];
            $where[] = "ede.estuf IN('" . implode("', '", $param['estuf']) . "')";
            $join['endereco'] = "JOIN entidade.endereco ede ON ede.endid = e.endid AND ede.endstatus = 'A'";
        }

        if ($param['muncod']) {
            $param['muncod'] = (array) $param['muncod'];
            $where[] = "ede.muncod IN('" . implode("', '", $param['muncod']) . "')";
            $join['endereco'] = " JOIN entidade.endereco ede ON ede.endid = e.endid AND ede.endstatus = 'A' ";
        }

        if ($param['empesfera']) {
            $param['empesfera'] = (array) $param['empesfera'];
            $where[] = "e.empesfera IN('" . implode("', '", $param['empesfera']) . "')";
        }

        if ($param['empvalorprevisto_menor']) {
            $where[] = "e.empvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_menor']);
        }

        if ($param['empvalorprevisto_maior']) {
            $where[] = "e.empvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_maior']);
        }

        if ($param['esdidsituaco']) {
            $where[] = "ed.esdid = " . $param['esdidsituaco'];
        }

        // Verifica se alguma supervisao foi homologada com atraso com base na data de termino da OS
        if($param['h_atrasada'] == "sim"){
            $where[] = "TRUE = (
                                (ed1.esdid IN (734, 756, 757) AND (os.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid)::date)) AND os.sosdttermino < NOW()
                                )";
        }
        if($param['h_atrasada'] == "nao"){
            $where[] = "FALSE = (
                                (ed1.esdid IN (734, 756, 757) AND (os.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid)::date)) AND os.sosdttermino < NOW()
                                )";
        }

        if ($param['esdidsupervisao']) {
            $w = '';
            if(count($param['esdidsupervisao']) > 0){
                foreach($param['esdidsupervisao'] as $k => $v){
                    if ($v == "nao_iniciado") {
                        $w = "ed1.esdid is null";
                        unset($param['esdidsupervisao'][$k]);
                    }
                }
                if(count($param['esdidsupervisao']) > 0){
                    if($w)
                        $w .= " OR ed1.esdid IN ( '" . implode('\',\'',$param['esdidsupervisao']) . "' ) ";
                    else
                        $w = "ed1.esdid IN ( '" . implode('\',\'',$param['esdidsupervisao']) . "' ) ";
                }
                $where[] = $w;
            }
        }

        if ($param['obraestuf']) {
            $param['obraestuf'] = (array) $param['obraestuf'];
            $where[] = "tes.estuf IN('" . implode("', '", $param['obraestuf']) . "')";
            $join[] = " JOIN territorios.estado tes ON tes.estuf = edr.estuf ";
        }

        if ($param['obramescod']) {
            $where[] = " mes.mescod = '" . $param['obramescod'] . "'";
        }

        if ($param['obramiccod']) {
            $where[] = " mic.miccod = '" . $param['obramiccod'] . "'";
        }

        if ($param['obramuncod']) {
            $where[] = " m.muncod = '" . $param['obramuncod'] . "'";
        }

        //obras2.supervisaoempresa pegar o campo suedtsupervisao e ver se a data de hj é maior
        if ($param['atrasado'] == "sim") {
            $where[] = " TRUE = ((d1.esdid IN (732, 733) AND os.sosdttermino < NOW()) OR
				            d1.esdid IS NULL)";
        }

        if ($param['atrasado'] == "nao") {
            $where[] = " FALSE = ((d1.esdid IN (732, 733) AND os.sosdttermino < NOW()) OR
				            d1.esdid IS NULL)
                            ";
        }

        //obras2.supervisaoempresa pegar o campo sueendcorreto, para fazer a verificação de endereço correto da obra/supervisão
        if ($param['endereco_obr'] == "sim") {
            $where[] = "se.sueendcorreto = 's'";
        }
        
        if ($param['sobid']) {
            $where[] = "se.sobid = $param[sobid]";
        }
        
        if ($param['endereco_obr'] == "nao") {
            $where[] = "se.sueendcorreto = 'n'";
        }
        if ($param['endereco_obr'] == "todos") {
            $where[] = "(se.sueendcorreto = 'n' OR se.sueendcorreto = 's' OR se.sueendcorreto is NULL)";
        }


//		$where[] = "ed.esdid = " . ESDID_OS_EXECUCAO;

        if (!possui_perfil(Array(PFLCOD_SUPER_USUARIO))) {

//			$arrEst = Array(PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_ESTADUAL,
//							PFLCOD_EMPRESA_CONTRATADA);
//			$arrObr = Array(PFLCOD_EMPRESA_CONTRATADA,
//							PFLCOD_SUPERVISOR_UNIDADE);
//			$arrOrg = Array(PFLCOD_ADMINISTRADOR,
//							PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_ESTADUAL,
//							PFLCOD_CONSULTA_TIPO_DE_ENSINO,
//							PFLCOD_SUPERVISOR_MEC);
//			$arrUni = Array(PFLCOD_AUDITOR_INTERNO,
//							PFLCOD_CADASTRADOR_INSTITUCIONAL,
//							PFLCOD_CONSULTA_UNIDADE,
//							PFLCOD_GESTOR_UNIDADE,
//							PFLCOD_SUPERVISOR_UNIDADE);

            $arrEmp = Array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                PFLCOD_EMPRESA_VISTORIADORA_GESTOR);

            $arrObr = Array(PFLCOD_EMPRESA_CONTRATADA/* ,
                      PFLCOD_SUPERVISOR_UNIDADE */);

//			$arrUni = Array(PFLCOD_SUPERVISOR_UNIDADE);

            $arrOrg = Array(PFLCOD_ADMINISTRADOR,
                PFLCOD_CADASTRADOR_INSTITUCIONAL,
                PFLCOD_CONSULTA_ESTADUAL,
                PFLCOD_CONSULTA_TIPO_DE_ENSINO,
                PFLCOD_SUPERVISOR_MEC,
                PFLCOD_GESTOR_MEC);

            $resp = array();
            $arPflcod = array();

//			if(possui_perfil($arrEst)){
//				$resp[] = "urs.estuf = end.estuf ";
//			}

            if (possui_perfil($arrEmp)) {
                $join[] = "JOIN obras2.supervisao_grupo_empresa sge ON sge.sgeid = os.sgeid AND sge.sgestatus != 'I'";
                $resp[] = "urs.entid = sge.entid";
//				$arPflcod = $arrEmp;
                $arPflcod = array_merge($arPflcod, $arrEmp);
            }
            if (possui_perfil($arrObr)) {
                $resp[] = "urs.empid = e.empid ";
//				$arPflcod = $arrObr;
                $arPflcod = array_merge($arPflcod, $arrObr);
            }
            if (possui_perfil($arrOrg)) {
                $resp[] = "urs.orgid = e.orgid ";
                $arPflcod = $arrOrg;
                $arPflcod = array_merge($arPflcod, $arrOrg);
            }
//			if(possui_perfil($arrUni)){
//				$resp[] = "urs.entid = e.entidunidade ";
////				$arPflcod = $arrUni;
//				$arPflcod = array_merge($arPflcod, $arrUni);
//			}

            if (possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) {
                $usuarioResp = new UsuarioResponsabilidade();
                $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
                $arEmpid = ($arEmpid ? $arEmpid : array(''));

                $resp[] = " urs.entid = e.entidunidade AND e.empid IN('" . implode("', '", $arEmpid) . "')";
                $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
            }

            if (count($resp)) {
                $arPflcod = array_unique($arPflcod);
                $join['usuarioresponsabilidade'] = "JOIN obras2.usuarioresponsabilidade urs 
                                                      ON urs.rpustatus = 'A' AND
                                                         urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                         urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND 
                                                         (" . implode(' OR ', $resp) . ")";
            }
        }

        $whereSupEmp = array();
        if (!possui_perfil(array(PFLCOD_SUPER_USUARIO,
                    PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                    PFLCOD_EMPRESA_VISTORIADORA_GESTOR,
                    PFLCOD_EMPRESA_CONTRATADA))) {
            $whereSupEmp[] = "ed.esdid = " . ESDID_VISTORIA_EMP_LAUDO;
        }

        if (possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
                    PFLCOD_EMPRESA_VISTORIADORA_GESTOR))) {
            $where[] = " ed.esdid IN (" . ESDID_OS_CONCLUIDA . ", " . ESDID_OS_EXECUCAO . ")";
        }

        $acao = "
                '<center><div style=\"width: 70px;\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/edit_on.gif\"
                        style=\"cursor: pointer; margin-left: 3px;\"
                        onclick=\"javascript: abreVistoriaEmpresa(\'' || e.empid || '\', \'' || os.sosid || '\', \'' || COALESCE(se.sueid::VARCHAR, '') || '\');\"
                        title=\"Ir para a Vistoria da Empresa\"
                    />'
                ||
                (CASE WHEN ed1.esdid NOT IN(" . WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO . ", " . WF_ESDID_LAUDO_SUPERVISAO_AGUARDANDO_HOMOLOGACAO . ", " . WF_ESDID_LAUDO_SUPERVISAO_CANCELADO . ") AND se.sueid IS NOT NULL
                        THEN 
                                '<img
                                                align=\"absmiddle\"
                                                src=\"/imagens/print.png\"
                                                style=\"cursor: pointer; margin-left: 3px;\"
                                                onclick=\"javascript: imprimirLaudo(\'' || se.sueid || '\',\'' || e.empid || '\');\"
                                                title=\"Imprimir Laudo\">'
                        ELSE
                                '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                END)
                ||
                '<img
                        align=\"absmiddle\"
                        src=\"/imagens/consultar.gif\"
                        style=\"cursor: pointer; margin-left: 3px;\"
                        onclick=\"javascript:abrirConsultaEmpresa(' || e.empid || ');\"
                        title=\"Consultar Obra\">'

                || 
                (CASE WHEN ed1.esdid NOT IN(" . WF_ESDID_LAUDO_SUPERVISAO_EM_CADASTRAMENTO . ")
                    THEN
                        '<img 
                            align=\"absmiddle\"
                            style=\"cursor: pointer; margin-left: 3px;\"
                            title=\"Imprimir questionário respondido\" 
                            onclick=\"imprimirQuestionarioRespondido(' || se.sueid || ', ' || os.sosid || ')\" 
                            src=\"../imagens/print.png\">'
                            ELSE
                                '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                END)
                ||        
                '</div></center>'
                ";
        $latlng = '';
        if($param['mapa']){
            $latlng = " ,

                --############### LATITUDE ###################### --

                -- Valores do IBGE convertidos em  decimal
                CASE WHEN (length (m.munmedlat)=8) THEN
                    CASE WHEN length(REPLACE('0' || m.munmedlat,'S','')) = 8 THEN
                        ((SUBSTR(REPLACE('0' || m.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || m.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || m.munmedlat,'S',''),1,2)::double precision))*(-1)
                    ELSE
                        (SUBSTR(REPLACE('0' || m.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || m.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || m.munmedlat,'N',''),1,2)::double precision)
                    END
                ELSE
                    CASE WHEN length(REPLACE(m.munmedlat,'S','')) = 8 THEN
                       ((SUBSTR(REPLACE(m.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(m.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(m.munmedlat,'S',''),1,2)::double precision))*(-1)
                    ELSE
                      0--((SUBSTR(REPLACE(m.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(m.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(m.munmedlat,'N',''),1,2)::double precision))
                    END
                END as latitude,
                --############### FIM LATITUDE ###################### --

                --############### LONGITUDE ###################### --
                    -- Valores do IBGE convertidos em  decimal
                   (SUBSTR(REPLACE(m.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(m.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
                 as longitude
                --############### FIM LONGITUDE ###################### --

            ";
        }

        $sql = "SELECT DISTINCT
                                    " . (($param['xls'] == '1') ? "" : "{$acao} AS acao,") . "

                                    o.obrid,
                                    e.empdsc AS empdsc,
                                    ( SELECT esd.esddsc 
                                      FROM obras2.obras obr 
                                      JOIN workflow.documento doc on doc.docid = obr.docid 
                                      JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
                                      WHERE obr.obridpai is null 
                                        AND obr.obrstatus = 'A' 
                                        AND obr.empid = e.empid 
                                        ORDER BY obrid DESC LIMIT 1) as situacao,
                                    ent3.entnome,
                                    os.sosnum,
                                    to_char( os.sosdttermino, 'DD/MM/YYYY')  as data,
                                    ed.esddsc,
                                    m.estuf || ' - ' || m.mundescricao as uf_muni,

                                    mic.micdsc,
                                    mes.mesdsc,
                                    COALESCE(ed1.esddsc,'Não Iniciada') AS situacao_supervisao,
                                    " . (($param['xls'] == '1') ? "to_char(se.suedtatualizacao, 'DD/MM/YYYY')||' ( '||DATE_PART('days', NOW() - se.suedtatualizacao)||' dia(s) )'" : "'<font ' ||
                                    CASE WHEN DATE_PART('days', NOW() - se.suedtatualizacao) <= 45 THEN
                                                    CASE WHEN e.emppercentultvistoriaempresa >= 100.00 THEN
                                                            ' TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                    ELSE
                                                            ' TITLE=\"Esta obra foi atualizada em até 45 dias\">'
                                                    END
                                    WHEN DATE_PART('days', NOW() - se.suedtatualizacao) > 45 AND DATE_PART('days', NOW() - se.suedtatualizacao) <= 60 THEN
                                           CASE WHEN e.emppercentultvistoriaempresa >= 100.00 THEN
                                                   ' TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                           ELSE
                                                   ' TITLE=\"Esta obra foi atualizada entre 45 e 60 dias\">'
                                           END
                                    WHEN DATE_PART('days', NOW() - se.suedtatualizacao) > 60 THEN
                                           CASE WHEN e.emppercentultvistoriaempresa >= 100.00 THEN
                                                   ' TITLE=\"Esta obra foi atualizada a mais de 60 dias\">'
                                           ELSE
                                                   ' TITLE=\"Esta obra está desatualizada\">'
                                           END
                                    END
                                    || to_char(se.suedtatualizacao, 'DD/MM/YYYY')||'</br>( '||DATE_PART('days', NOW() - se.suedtatualizacao)||' dia(s) ) </font>'") . "
                                     AS dataUltVistoria,

                                    TO_CHAR(se.suedtsupervisao, 'dd/mm/yyyy'),

                                    ( SELECT
                                        CASE
                                            WHEN SUM(icovlritem) > 0 THEN
                                            ROUND( (SUM( spivlrfinanceiroinfsupervisor ) /  SUM(icovlritem)) * 100, 2)
                                            ELSE
                                            0
                                        END AS total
                                        FROM
                                        obras2.itenscomposicaoobra i
                                        JOIN obras2.cronograma cro ON cro.croid = i.croid AND cro.crostatus IN ('A', 'H') AND su.croid = cro.croid
                                        LEFT JOIN
                                        obras2.supervisaoitem sic ON sic.icoid = i.icoid
                                        AND sic.supid = su.supid
                                        AND sic.icoid IS NOT NULL
                                        AND sic.ditid IS NULL
                                        WHERE
                                        i.icostatus = 'A' AND
                                        i.relativoedificacao = 'D' AND
                                        cro.obrid = su.obrid AND i.obrid = cro.obrid
                                    ) as percentualm,

                                    CASE WHEN o.tpoid IN (104, 105) THEN 'SIM' ELSE 'NÃO' END as obra_mi,
                                    TO_CHAR( (select MAX(htddata) as c
                                                  FROM workflow.documento d
                                                  JOIN workflow.historicodocumento h ON h.docid = d.docid AND h.aedid = 1726
                                                  WHERE d.docid = se.docid
                                                  ), 'DD/MM/YYYY') as data_homol
                                    ";
                                    if($param['xlsparalizada'] == '1'){
                                        $where[] = "tpl.tplid = 4";
                                        $sql .= ", tpl.tpldsc, se.sueobservacao ,qoprespostaum,qoprespostadois,qoprespostatres,qoprespostaquatro,qoprespostacinco ";
                                    }

                                    if($param['xls'] == '1'){
                                        $sql .= "
                                            ,CASE WHEN se.sueendcorreto = 's' THEN 'SIM' WHEN se.sueendcorreto = 'n' THEN 'NÃO' ELSE 'NÃO INFORMADO' END,
                                            edrS.endcep as endcepS,
                                            edrS.endlog as endlogS,
                                            edrS.endnum as endnumS,
                                            edrS.endcom as endcomS,
                                            mS.mundescricao as mundescricaoS,
                                            mS.estuf as estufS,
                                            edrS.medlatitude as medlatitudeS,
                                            edrS.medlongitude as medlongitudeS,
                                            usu.usunome,
                                            edr.endcep,
                                            edr.endlog,
                                            edr.endnum,
                                            edr.endcom,
                                            m.mundescricao,
                                            m.estuf,
                                            edr.medlatitude,
                                            edr.medlongitude
                                        ";
                                    }


                                    $sql .= "


                                    $latlng

                                            FROM
                                    obras2.empreendimento e

                            LEFT JOIN obras2.obras                            o ON o.empid    = e.empid AND o.obrstatus = 'A' AND o.obridpai IS NULL

                            -- Endereço Obra
                            LEFT JOIN entidade.endereco                ende ON ende.endid  = o.endid
                            LEFT JOIN territorios.municipio            mun ON mun.muncod   = ende.muncod

                            JOIN obras2.supervisao_os_obra              oo ON oo.empid   = e.empid  AND oo.soostatus = 'A'
                            JOIN obras2.supervisao_os                   os ON os.sosid   = oo.sosid AND os.sosstatus = 'A'
                            JOIN workflow.documento                      d ON d.docid    = os.docid					
                            JOIN workflow.estadodocumento               ed ON ed.esdid   = d.esdid                            
                            LEFT JOIN obras2.tipologiaobra             tpo ON tpo.tpoid  = e.tpoid
                            LEFT JOIN entidade.endereco                edr ON edr.endid  = e.endid
                            LEFT JOIN territorios.municipio              m ON m.muncod   = edr.muncod
                            LEFT JOIN territorios.mesoregiao           mes ON mes.mescod = m.mescod
                            LEFT JOIN territorios.microregiao          mic ON mic.miccod = m.miccod
                            LEFT JOIN obras2.supervisao_grupo_empresa sge1 ON sge1.sgeid = os.sgeid
                            LEFT JOIN entidade.entidade               ent3 ON ent3.entid = sge1.entid
                            LEFT JOIN obras2.supervisaoempresa          se ON se.suestatus = 'A' AND se.empid = e.empid and  se.sosid   = os.sosid
                            LEFT JOIN entidade.endereco                edrS ON edrS.endid  = o.endid
                            LEFT JOIN territorios.municipio              mS ON mS.muncod   = edrS.muncod
                            LEFT JOIN workflow.documento                d1 ON d1.docid   = se.docid
                            LEFT JOIN workflow.estadodocumento         ed1 ON ed1.esdid  = d1.esdid
                            LEFT JOIN obras2.supervisao                 su ON su.sueid   = se.sueid AND su.supstatus = 'A' -- AND su.obrid = o.obrid

                            LEFT JOIN obras2.questionarioobraparalisada que on  su.obrid = que.obrid
                            LEFT JOIN obras2.tipoparalisacao tpl on se.tplid = tpl. tplid

                            LEFT JOIN seguranca.usuario usu ON usu.usucpf = se.usucpf
                            " . (count($join) ? implode(" ", $join) : "") . "
                            WHERE
                                    e.empstatus = 'A' --AND e.empidpai IS NULL
                                    " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
                            ORDER BY
                                    2";
//       ver(simec_htmlentities($sql), d);
        return $sql;
    }

    public function listaObras(Array $param = array()) {

        $where = Array("obr.obrstatus = 'A'", "obr.empid = " . $this->empid);

        if ($param['not(obridpai)']) {
            $where[] = "obr.obridpai IS NULL";
        }
        if ($param['tobid']) {
            $where[] = ' tob.tobid = ' . $_POST['tobid'];
        }
        if ($param['cloid']) {
            $where[] = ' clo.cloid = ' . $_POST['cloid'];
        }

        $acao = "'<center>
			    	<img
				    	align=\"absmiddle\"
				    	src=\"/imagens/alterar.gif\"
				    	style=\"cursor: pointer\"
				    	onclick=\"javascript: alterarObr(\'' || obr.obrid || '\');\"
				    	title=\"Alterar Objeto\">
			    	<img
				    	align=\"absmiddle\"
				    	src=\"/imagens/excluir.gif\"
				    	style=\"cursor: pointer; margin-left: 3px;\"
				    	onclick=\"javascript: excluirObr(\'' || obr.obrid || '\');\"
				    	title=\"Excluir Objeto\">
		    	</center>'";

        $sql = "SELECT
    				{$acao} as id,
    				'<a href=\"javascript: alterarObr(\'' || obr.obrid || '\');\" title=\"Alterar objeto\">(' || obr.obrid || ') ' || obrnome || '</a>',
    				tobdesc,
    				clodsc
    			FROM
    				obras2.obras obr
    			INNER JOIN obras2.tipoobra tob ON tob.tobid = obr.tobid
    			INNER JOIN obras2.classificacaoobra clo ON clo.cloid = obr.cloid
    			WHERE
    				" . implode(' AND ', $where) . "
    			ORDER BY
    				2";
        $dados = $this->carregar($sql);

        return is_array($dados) ? $dados : Array();
    }

    public function listaCombo(Array $param = array()) {
        $where = array();

        switch (true) {
            case!empty($param['orgid']):
                $where[] = "orgid = {$param['orgid']}";
                break;
        }

        $sql = "SELECT
                        empid AS codigo,
                        empdsc AS descricao
                FROM
                        obras2.empreendimento
                WHERE
                        empstatus = 'A'" .
                (count($where) ? " AND " . implode(" AND ", $where) : "") .
                "ORDER BY
                        empdsc;";

        $dados = $this->carregar($sql);

        return (is_array($dados) ? $dados : array());
    } 

    public function montaCabecalho() {

        $endereco = new Endereco($this->endid);

        $sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '" . $endereco->muncod . "'";
        $municipio = $this->pegaUm($sql);

        $sql = "SELECT orgdesc FROM obras2.orgao WHERE orgid = '" . $this->orgid . "'";
        $orgdesc = $this->pegaUm($sql);

        if ($this->preid) {
            $sql = "SELECT
                            muncod, predescricao
                    FROM
                            obras.preobra
                    WHERE
                            preid = {$this->preid}";
            $dado = $this->pegaLinha($sql);

            $sql = "SELECT 
                            so.sbaid, so.sobano 
                    FROM 
                    par.subacaoobra so 
                    JOIN par.subacao s on s.sbaid = so.sbaid AND 
                                                              s.sbastatus = 'A'
                    WHERE so.preid = {$this->preid}";
            $dadoPreObra = $this->pegaLinha($sql);

            $href = ( $dadoPreObra ? 
                    "/par/par.php?modulo=principal/subacaoObras&acao=A&preid={$this->preid}&sbaid={$dadoPreObra['sbaid']}&ano={$dadoPreObra['sobano']}" : 
                    "/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid={$this->preid}&muncod={$dado['muncod']}"
                    );

            $htm = "<tr>
                            <td class=\"SubTituloDireita\" width=\"20%\"><b>Pré-obra:</b></td>
                            <td>
                                    <a href=\"javascript:janela('{$href}',800,600,'preobra')\">
                                    ({$this->preid}) {$dado['predescricao']}
                                    </a>
                            </td>
                     </tr>";
        } elseif ($this->demid) {
            $demanda = new Demanda($this->demid);
            $htm = "<tr>
                            <td class=\"SubTituloDireita\" width=\"20%\"><b>Pré-obra:</b></td>
                            <td>
                                    <a target=\"preobra\" href=\"?modulo=principal/preobra/cadDemanda&acao=A&demid={$demanda->demid}\">
                                    ({$demanda->demid}) {$demanda->demnome}
                                    </a>
                            </td>
                     </tr>";
        }

        if ($this->obrid_1) {
            $htm2 = "<tr id='tr_obras1'>
                            <td class=\"SubTituloDireita\" width=\"20%\"><b>&nbsp;</b></td>
                            <td>
                                    Para acessar o obras 1 e ver o histórico desta obra 
                                    <a href=\"/obras/obras.php?modulo=principal/cadastro&acao=A&obrid={$this->obrid_1}\">
                                    clique aqui
                                    </a>.
                            </td>
                     </tr>";
        }
        
        $obr  = new Obras();
        $htm3 = $obr->getLinhaTabelaPercentExecutado($_SESSION['obras2']['obrid']);
        
        echo '<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">';
        
        if (isset($_SESSION['obras2']['obrid']) && !empty($_SESSION['obras2']['obrid'])){
            echo montaBarraFerramentas($_SESSION['obras2']['obrid']);
        }
        echo '  <tr>
                    <td class="SubTituloDireita" width="20%"><b>Tipo de ensino:</b></td>
                    <td>
                        '.$orgdesc.'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Empreendimento:</b></td>
                    <td>
                        ('.$this->empid.') '.$this->empdsc.'
                    </td>
                </tr>
                '.$htm.'
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Município/UF:</b></td>
                    <td>
                    '.$municipio.' / '.$endereco->estuf.'
                    </td>
                </tr>
                '.$htm2.' 
                '.$htm3.' 
            </table>';

}

public function dadosResumo($empid) {
    $sql = "SELECT
                        e.*,
                        TO_CHAR(e.empdtultvistoria, 'dd/mm/YYYY') AS empdtultvistoria,
                        en.estuf,
                        en.endlog,
                        en.endcom,
                        m.mundescricao,
                        tpo.tpodsc,
                        ed.esddsc,
                        eu.entnome AS entnomeunidade,
                        ec.entnome AS entnomecampus,
                        oc.somavlrcontrato
                FROM
                        obras2.empreendimento e
                        JOIN entidade.endereco 				en ON en.endid = e.endid 
                        LEFT JOIN territorios.municipio		m ON m.muncod = en.muncod					
                        JOIN obras2.tipologiaobra 			tpo ON tpo.tpoid = e.tpoid
                        LEFT JOIN workflow.documento		d ON d.docid = e.docid					
                        LEFT JOIN workflow.estadodocumento	ed ON ed.esdid = d.esdid					
                        LEFT JOIN entidade.entidade 		eu ON eu.entid = e.entidunidade
                        LEFT JOIN entidade.entidade 		ec ON ec.entid = e.entidcampus
                        LEFT JOIN (SELECT 
                    SUM(ocrvalorexecucao) AS somavlrcontrato,
                    o.empid
               FROM
                    obras2.obras o 
                    JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND
                                                                                    oc.ocrstatus = 'A'
                    JOIN obras2.contrato c ON c.crtid = oc.crtid AND
                                                                      c.crtidpai IS NULL AND
                                                                      c.crtstatus = 'A'				
               WHERE
                    o.obrstatus = 'A'
               GROUP BY
                    empid) oc ON oc.empid = e.empid
                WHERE
                        e.empid = {$empid}";

    $dado = $this->pegaLinha($sql);
    return ($dado ? $dado : array());
}

public function verificaEquivalenciaContratoCronogramaDoObjetoPorEmpid($empid) {
    $obras = new Obras();
    $param = array();
    $param['not(obridpai)'] = true;
    $arObrid = $obras->pegaIdObraPorEmpid($empid, $param);

    $retorno = true;
    if (count($arObrid)) {
        $obraContrato = new ObrasContrato();
        $itemComposicao = new ItensComposicaoObras();
        foreach ($arObrid as $obrid) {
            $ocrvalorexecucao = $obraContrato->getValorContrato($obrid);
            $somaIcoValor = $itemComposicao->getSomaEtapaByObra($obrid);

            if ($ocrvalorexecucao != $somaIcoValor || ( empty($ocrvalorexecucao) || empty($somaIcoValor) )) {
                $retorno = false;
                break;
            }
        }
    } else {
        $retorno = false;
    }

    return $retorno;
}

public function pegaEstadoObra() {

    $sql = "SELECT 
                        esd.esdid,
                        esd.esddsc
                FROM 
                        workflow.documento doc 
                INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
                WHERE 
                        docid = " . $this->docid;

    return $this->pegalinha($sql);
}

public function buscaEmpresaOS() {

    $sql = "SELECT
                    sosid
            FROM
                    obras2.supervisao_os_obra
            WHERE
                    soostatus = 'A' AND
                    empid = {$this->empid}";
    $sosid = $this->pegaUm($sql);
    $os = new Supervisao_Os($sosid);
    if ($os->sgeid != '') {
        $sql = "SELECT
                        replace(to_char(e.entnumcpfcnpj::int8,'00{}000{}000/0000-00'),'{}','.') as cnpj,
                        e.entnome AS descricao
                FROM
                        obras2.supervisao_grupo_empresa sge
                JOIN entidade.entidade e ON e.entid = sge.entid
                WHERE
                        sgestatus != 'I'
                        AND sge.sgeid = {$os->sgeid}
                ORDER BY
                        entnome";
        return $this->pegaLinha($sql);
    }
}

public function pegaSituacaoWf($empid) {
    $sql = "SELECT
                ARRAY_TO_STRING( ARRAY(SELECT 
                     ed.esddsc 
                FROM 
                     obras2.empreendimento e
                JOIN obras2.obras o ON o.empid = e.empid
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                WHERE
                     e.empid = {$empid}
                 ), ', ') AS esddsc";
    return $this->pegaUm($sql);
}

/*
 * Recupera municípios do(s) empreendimento(s)
 *
 * @param array||integer - $emp (array de empids ou um único empid)
 * @return array - array de municipios
 */
public function pegaMuncodPorEmpid($emp)
{
    $municipios = array();
    if($emp){
        $aEmpid = (array) $emp;
        $sql = "select distinct m.muncod, m.mundescricao, m.estuf
                from obras2.empreendimento e
                        inner join entidade.endereco ed on ed.endid = e.endid
                        inner join territorios.municipio m on m.muncod = ed.muncod
                WHERE empid in ('" . implode("', '", $aEmpid) . "')";

        $municipios =  $this->carregar($sql);
    }

    return $municipios ? $municipios : array();
}

}
            