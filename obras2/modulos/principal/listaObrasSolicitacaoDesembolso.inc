<?php
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';


#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['solicitacaodesembolso']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}


if ($_POST['req'] == 'apagar' && !empty($_POST['sldid'])) {
    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $solicitacaoDesembolso = new SolicitacaoDesembolso($_POST['sldid']);
        $solicitacaoDesembolso->sldstatus = 'I';
        $solicitacaoDesembolso->salvar();
        $solicitacaoDesembolso->commit();
        die('<script type="text/javascript">
            alert(\'Operação realizada com sucesso!\');
            location.href=\'?modulo=principal/listaObrasSolicitacaoDesembolso&acao=A\';
         </script>');
    }
}


switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $estuf = $_REQUEST['estuf'];
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>
        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";


monta_titulo( 'Lista de Solicitações de Desembolso', '');

if(empty($_POST) && !empty($_SESSION['obras2']['solicitacaodesembolso']['filtros']))
    $_POST = $_SESSION['obras2']['solicitacaodesembolso']['filtros'];
else
    $_SESSION['obras2']['solicitacaodesembolso']['filtros'] = $_POST;

//ver($_SESSION['obras2'], d);

extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form method="post" name="formListaObraSolicitacaoDesembolso" id="formListaObraSolicitacaoDesembolso">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="sldid" id="sldid" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
			<td>
				<?=campo_texto('obrbuscatexto','N','S','',70,100,'','', '', '', '', 'id="obrbuscatexto"');?>
			</td>
		</tr>

        <tr>
            <td class="SubTituloDireita" width="15%">ID da Solicitação:</td>
            <td>
                <?=campo_texto('sldid','N','S','',70,100,'','', '', '', '', 'id="sldid"');?>
            </td>
        </tr>


        <tr>
            <td class="SubTituloDireita">
                Situação do Obra:
            </td>
            <td>
                <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdid) && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>

            <td class="SubTituloDireita">
                Tipologia:
            </td>
            <td>
                <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);


                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($tpoid) && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Programa:
            </td>
            <td>
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Fonte:
            </td>
            <td>
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>


        <? if(possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))): ?>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<td>
				<?php 
				$sql = Array(Array('codigo'=>'E', 'descricao'=>'Estadual'),
							 Array('codigo'=>'M', 'descricao'=>'Municipal'));
				$db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
				?>
			</td>
		</tr>
            <tr>

                <td class="SubTituloDireita">UF(s):</td>
                <td>

                    <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
                        <?php  $uf = new Estado();
                        foreach ($uf->listaCombo() as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($estuf) && in_array($key['codigo'], $estuf)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>


                </td>


            </tr>
            <tr>
                <td class="SubTituloDireita">Municípios(s):</td>
                <td class="td_municipio">
                    <?php if (!empty($estuf)) { ?>

                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                            <?php   $municipio = new Municipio();
                            foreach ($municipio->listaComboMulti($estuf) as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    <?php } ?>
                </td>
            </tr>

		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Situação:</td>
			<td>
                    <?php
                    $dado = $db->carregar('
                                            SELECT
                                              \'999\' codigo,
                                              \'Aguardando Análise FNDE\' descricao
                                            UNION
                                            SELECT
                                              esdid codigo,
                                              esddsc descricao
                                            FROM workflow.estadodocumento
                                            WHERE
                                              tpdid = '.TPDID_SOLICITACAO_DESEMBOLSO.'
                                            ');

                    ?>

                <select name="esdidsituaco[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
			</td>
		</tr>

        <tr>
            <td class="SubTituloDireita">Convênio/Termo:</td>
            <td>
                Número:&nbsp;
                <?php
                echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Processo:</td>
            <td>
                Número:&nbsp;
                <?php
                echo campo_texto('processo', 'N', 'S', '', 20, 20, '#####.######/####-##', '', 'right', '', 0, '');
                ?>
                Ano:&nbsp;
                <?php
                echo campo_texto('ano_processo', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Execução:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        for ($i = 0; $i <= 100; $i++) {
                            $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                        }
                        $percentualinicial = $_POST['percentualinicial'];
                        $percentualfinal = $_POST['percentualfinal'];
                        $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                        print '<td>';
                        $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Pago:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        $percentualpagoPagoinicial = $_POST['percentualpagoinicial'];
                        $percentualpagofinal = $_POST['percentualpagofinal'];
                        $percfinal = $percentualpagofinal == '' ? 100 : $percentualpagofinal;
                        print '<td>';
                        $db->monta_combo("percentualpagoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualpagoinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualpagofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualpagofinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Solicitado:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        $percentualsolicitadoinicial = $_POST['percentualsolicitadoinicial'];
                        $percentualsolicitadofinal = $_POST['percentualsolicitadofinal'];
                        $percfinal = $percentualsolicitadofinal == '' ? 100 : $percentualsolicitadofinal;
                        print '<td>';
                        $db->monta_combo("percentualsolicitadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadoinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualsolicitadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualsolicitadofinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" style="width: 190px;">% Aprovado:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        $percentualaprovadoinicial = $_POST['percentualaprovadoinicial'];
                        $percentualaprovadofinal = $_POST['percentualaprovadofinal'];
                        $percfinal = $percentualaprovadofinal == '' ? 100 : $percentualaprovadofinal;
                        print '<td>';
                        $db->monta_combo("percentualaprovadoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadoinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualaprovadofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualaprovadofinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>
        <? else: ?>
            <tr>
                <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
                <td>
                    <?php
                    $dado = $db->carregar('
                                            SELECT
                                              \'999\' codigo,
                                              \'Aguardando Análise FNDE\' descricao
                                            UNION
                                            SELECT
                                              esdid codigo,
                                              esddsc descricao
                                            FROM workflow.estadodocumento
                                            WHERE
                                              tpdid = '.TPDID_SOLICITACAO_DESEMBOLSO.'
                                              AND esdid NOT IN (' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA . ', ' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI . ', ' . ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL . ')');

                    ?>

                    <select name="esdidsituaco[]" class="chosen-select" multiple data-placeholder="Selecione">
                        <?php
                        foreach ($dado as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($esdidsituaco) && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>

                </td>
            </tr>
        <? endif; ?>

        <tr>
            <td class="SubTituloDireita">Situação do Pedido:</td>
            <td>
                <?php

                $dado = Array(Array('codigo'=>'Solicitação Aprovada', 'descricao'=>'Solicitação Aprovada'),
                             Array('codigo'=>'agamento Solicitado', 'descricao'=>'Pagamento Solicitado'),
                             Array('codigo'=>'Efetivado', 'descricao'=>'Efetivado'));
                ?>
                <select name="sitpedido[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if (isset($sitpedido) && in_array($key['codigo'], $sitpedido)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>

        <tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>

<?php

if ( $obrbuscatexto ){
	$where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') ) ";
}

if( $empesfera ){
	$empesfera = (array) $empesfera;
	$where[] = "emp.empesfera IN('" . implode("', '", $empesfera) . "')";
}


if ($_POST['sldid'])
    $where['sldid'] = "sv.sldid IN ('{$_POST['sldid']}')";

if ($_POST['sitpedido'] && $_POST['sitpedido'][0] != '')
    $where['sitpedido'] = "ps.situacao_pagamento IN ('" . implode("', '", $_POST['sitpedido']) . "')";

if ($_POST['estuf'] && $_POST['estuf'][0] != '')
    $where['estuf'] = "m.estuf IN ('" . implode("', '", $_POST['estuf']) . "')";
if ($_POST['muncod'] && $_POST['muncod'][0] != '')
    $where['muncod'] = "m.muncod IN ('" . implode("', '", $_POST['muncod']) . "')";
if ($_POST['processo'])
    $where['processo'] = "Replace(Replace(Replace( TRIM(p_conv.pronumeroprocesso),'.',''),'/',''),'-','') = Replace(Replace(Replace( '{$_POST['processo']}','.',''),'/',''),'-','')";
if ($_POST['ano_processo'])
    $where['ano_processo'] = "substring(Replace(Replace(Replace( p_conv.pronumeroprocesso,'.',''),'/',''),'-','') from 12 for 4) = '{$_POST['ano_processo']}'";
if ($_POST['convenio'])
    $where['convenio'] = "p_conv.termo_convenio = '{$_POST['convenio']}'";
if ($_POST['ano_convenio'])
    $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_POST['ano_convenio']}'";
if ($_POST['esdid'] && $_POST['esdid'][0] != '')
    $where['esdid'] = "dc.esdid IN (" . implode(', ', $_POST['esdid']) . ")";
if ($_POST['tpoid'] && $_POST['tpoid'][0] != '')
    $where['tpoid'] = "o.tpoid IN ('" . implode("', '", $_POST['tpoid']) . "')";
if ($_POST['prfid'] && $_POST['prfid'][0] != '')
    $where['prfid'] = "emp.prfid IN(" . implode(',', $_POST['prfid']) . ")";
if ($_POST['tooid'] && $_POST['tooid'][0] != '')
    $where['tooid'] = "o.tooid IN(" . implode(',', $_POST['tooid']) . ")";

if ($_POST['percentualinicial'] != '')
    $where['percentualinicial'] = "COALESCE(obrpercentultvistoria, 0) >= " . $_POST['percentualinicial'];
if ($_POST['percentualfinal'] != '')
    if ($_POST['percentualfinal'] < 100)
        $where['percentualfinal'] = "COALESCE(obrpercentultvistoria, 0) <= " . $_POST['percentualfinal'];

if ($_POST['percentualpagoinicial'] != '')
    $where['percentualpagoinicial'] = "COALESCE(((p.totalpago * 100) / p.vlrobra), 0) >= " . $_POST['percentualpagoinicial'];
if ($_POST['percentualpagofinal'] != '')
    if ($_POST['percentualpagofinal'] < 100)
        $where['percentualpagofinal'] = "COALESCE(((p.totalpago * 100) / p.vlrobra), 0) <= " . $_POST['percentualpagofinal'];

if ($_POST['percentualsolicitadoinicial'] != '')
    $where['percentualsolicitadoinicial'] = "COALESCE(sv.sldpercsolicitado, 0) >= " . $_POST['percentualsolicitadoinicial'];
if ($_POST['percentualsolicitadofinal'] != '')
    if ($_POST['percentualsolicitadofinal'] < 100)
        $where['percentualsolicitadofinal'] = "COALESCE(sv.sldpercsolicitado, 0) <= " . $_POST['percentualsolicitadofinal'];

if ($_POST['percentualaprovadoinicial'] != '')
    $where['percentualaprovadoinicial'] = "COALESCE(sv.sldpercpagamento, 0) >= " . $_POST['percentualaprovadoinicial'];
if ($_POST['percentualaprovadofinal'] != '')
    if ($_POST['percentualaprovadofinal'] < 100)
        $where['percentualaprovadofinal'] = "COALESCE(sv.sldpercpagamento, 0) <= " . $_POST['percentualaprovadofinal'];

if ($esdidsituaco){
    if (!possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))) {
        if(in_array(999, $esdidsituaco) ) {
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI;
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL;
            $esdidsituaco[] = ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA;
            $where[] = " d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
        } else {
            $where[] = "  d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
        }

    } else {
        $where[] = "  d.esdid IN (" . implode(',', $esdidsituaco) . ") ";
    }
}

if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {

    $arrObr = array(
        PFLCOD_EMPRESA_VISTORIADORA_FISCAL,
        PFLCOD_EMPRESA_VISTORIADORA_GESTOR
    );

    $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

    $arrOrg = Array(PFLCOD_ADMINISTRADOR,
        PFLCOD_CADASTRADOR_INSTITUCIONAL,
        PFLCOD_CONSULTA_TIPO_DE_ENSINO,
        PFLCOD_SUPERVISOR_MEC,
        PFLCOD_GESTOR_MEC);

    $arrEst = Array(PFLCOD_CONSULTA_ESTADUAL);

    $resp = array();
    $arPflcod = array();
    $orWhere = array();

    if (possui_perfil($arrEst)) {
        $arPflcod = array_merge($arPflcod, $arrEst);
        $orWhere['estuf'] = "m.estuf IN ( SELECT estuf FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.estuf = ed.estuf)";
    }

    if (possui_perfil($arrObr)) {
        $arPflcod = array_merge($arPflcod, $arrObr);
        $orWhere['empid'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.empid = e.empid)";
    }

    if (possui_perfil($arrOrg)) {
        $arPflcod = array_merge($arPflcod, $arrOrg);
        $orWhere['orgid'] = "emp.orgid IN ( SELECT urs.orgid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.orgid = emp.orgid)";
    }

    if (possui_perfil($arrUni)) {
        $arPflcod = array_merge($arPflcod, $arrUni);
        $orWhere['entidunidade'] = "emp.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = emp.entidunidade)";
    }

    if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_CONSULTA_UNIDADE))) {
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
        $arEmpid = ($arEmpid ? $arEmpid : array(0));

        $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
        $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

        $orWhere['sup'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = emp.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
    }

}

$esddsc = (possui_perfil(array(PFLCOD_GESTOR_MEC, PFLCOD_SUPER_USUARIO))) ? "e.esddsc" : "CASE WHEN e.esdid IN (".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_TECNICA.", ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_DOCUMENTAL." , ".ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_ANALISE_REI.") THEN 'Aguardando Análise FNDE' ELSE e.esddsc END";

$acao = '';
if ($_POST['req'] != 'excel') {

    if(possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
        $excluir = "<img
                        align=\"absmiddle\"
                        src=\"/imagens/excluir.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: excluirSolicitacao(\'' ||sv.sldid || '\');\"
                        title=\"Excluir\">";
    }

    $acao = "'<center style=\"width:60px\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/icone_lupa.png\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: abreSolicitacao(\'' || sv.sldid  || '\');\"
                        title=\"Alterar Solicitação\">
                    <img
                        align=\"absmiddle\"
                        src=\"/imagens/alterar.gif\"
                        style=\"cursor: pointer\"
                        onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                        title=\"Alterar Obra\">
                    $excluir
                 </center>' AS acao,";
}

$sql = "
    SELECT * FROM (


            SELECT DISTINCT ON (sv.sldid)
                $acao
                sv.sldid,
                o.obrid,
                o.obrnome,
                p_conv.pronumeroprocesso,
                p_conv.termo_convenio,
                p_conv.ano_termo_convenio,
                eo.esddsc as esddsc1,
                ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) as percentual_execucao,
                (p.totalpago * 100) / p.vlrobra as totalpago,
                sv.sldpercsolicitado,
                sv.sldpercpagamento,
                m.estuf,
                m.mundescricao,
                sv.sldjustificativa,
                u.usunome usunome1,
                TO_CHAR(sv.slddatainclusao, 'DD/MM/YYYY') slddatainclusao,
                $esddsc,
                (
                    SELECT ud.usunome FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
                (
                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as htddata,
                (
                    SELECT c.cmddsc FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as cmddsc,
                ps.situacao_pagamento
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')

            LEFT JOIN workflow.documento dc ON dc.docid = o.docid
            LEFT JOIN workflow.estadodocumento eo ON eo.esdid = dc.esdid
            LEFT JOIN par.v_pagamento_total_por_obra p ON p.preid = o.preid
            LEFT JOIN (SELECT * FROM obras2.vm_termo_convenio_obras WHERE termo_convenio IS NOT NULL) as p_conv ON p_conv.obrid = o.obrid

            LEFT JOIN (
                    SELECT
                        sldid,
                        CASE WHEN situacao_pagamento IN ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA') THEN 'Solicitação Aprovada'
                        WHEN situacao_pagamento IN ('Enviado ao SIGEF', '6 - VALA SIAFI', '0 - AUTORIZADO') THEN 'Pagamento Solicitado'
                        WHEN situacao_pagamento IN ('2 - EFETIVADO') THEN 'Efetivado'
                        ELSE ''
                        END as situacao_pagamento
                    FROM (

                        select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                            from par.pagamentodesembolsoobras pdo
                            inner join par.pagamentoobra po on po.pobid = pdo.pobid
                            inner join par.pagamento pg on pg.pagid = po.pagid
                            inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                        where pdo.pdostatus = 'A'
                        UNION ALL
                        select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                            from par.pagamentodesembolsoobras pdo
                            inner join par.pagamentoobrapar pop on pop.popid = pdo.popid
                            inner join par.pagamento pg on pg.pagid = pop.pagid
                            inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                            where pdo.pdostatus = 'A'
                    ) as p
            ) ps ON ps.sldid = sv.sldid

            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid
            WHERE sv.sldstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ',$where) : "") . "
            " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
            ) as f ORDER BY 1
            ";


if ($_POST['req'] == 'excel') {
    $cabecalho = array('ID Solicitação', 'ID Obra', 'Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Situação', '% Execução', '% Pago', '% Solicitado', '% Aprovado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta', 'Situação do Pedido');
    $db->sql_to_xml_excel($sql, 'relatorioListaObjetosObras', $cabecalho, '');
} else {
    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'Nº Processo', 'Nº Termo/Convênio', 'Ano Termo/Convênio', 'Situação', '% Execução', '% Pago', '% Solicitado', '% Aprovado', 'UF', 'Município', 'Justificativa', 'Inserido Por', 'Data de Cadastro',
        'Situação do Deferimento', 'Última tramitação', 'Data da Resposta', 'Observação da Resposta', 'Situação do Pedido');
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', null, "formulario");
}


?>
<script type="text/javascript">
    
$(document).ready(function (){
	$('.pesquisar').click(function (){
		$('#req').val('');
		$('#formListaObraSolicitacaoDesembolso').submit();
	});

    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    });
});

function excluirSolicitacao(sldid) {
    if (confirm('Deseja apagar esta solicitação?')) {
        $('#sldid').val(sldid);
        $('#req').val('apagar');
        $('#formListaObraSolicitacaoDesembolso').submit();
    }
}

function alterarObr(obrid) {
    location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function abreSolicitacao(sldid){
	windowOpen('?modulo=principal/popupSolicitarDesembolso&acao=A&sldid=' + sldid,'telaSolicitacaoDesembolso','height=700,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

$('.btnEexcel').click(function () {
    $('#req').val('excel');
    $('#formListaObraSolicitacaoDesembolso').submit();
});

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
			  url  		 : url,
			  type 		 : 'post',
			  data 		 : {ajax  : 'municipio', 
			  		  	    estuf : estuf},
			  dataType   : "html",
			  async		 : false,
			  beforeSend : function (){
			  	divCarregando();
				td.find('select option:first').attr('selected', true);
			  },
			  error 	 : function (){
			  	divCarregado();
			  },
			  success	 : function ( data ){
			  	td.html( data );
			  	divCarregado();
			  }
		});	
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true)
						 .attr('disabled', true);
	}			
}
//-->


function carregarMunicipio(estuf) {

    var td = $('.td_municipio');
    if (estuf != '') {
        var url = location.href;
        $.ajax({
            url: url,
            type: 'post',
            data: {
                ajax: 'municipio',
                estuf: values
            },
            dataType: "html",
            async: false,
            beforeSend: function () {
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error: function () {
                divCarregado();
                alert(2);
            },
            success: function (data) {
                td.html(data);
                divCarregado();
            }
        });
    } else {
        td.find('select option:first').attr('selected', true);
        td.find('select').attr('selected', true)
            .attr('disabled', true);
    }
}
</script>

<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }
</style>



<script>
    $1_11(document).ready(function () {
        $1_11('select[name="tobid[]"]').chosen();
        $1_11('select[name="prfid[]"]').chosen();
        $1_11('select[name="tooid[]"]').chosen();
        $1_11('select[name="estuf[]"]').chosen();
        $1_11('select[name="muncod[]"]').chosen();
        $1_11('select[name="esdid[]"]').chosen();
        $1_11('select[name="tpoid[]"]').chosen();
        $1_11('select[name="esdidsituaco[]"]').chosen();
        $1_11('select[name="sitpedido[]"]').chosen();

        $1_11(".estados").chosen().change(function (e, params) {
            values = $1_11(".estados").chosen().val();
            carregarMunicipio(values);
        });
    });
</script>