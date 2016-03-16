<?php
include '_funcoes_empenho.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
//$tabela_execucao = monta_tabela_execucao( TIPO_OBRAS_PAC );
monta_titulo( $titulo_modulo, $tabela_execucao );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<script>
function carregarListaEmpenhoPagamento(pro, obj) {
	var tabela = obj.parentNode.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		var col0 = nlinha.insertCell(0);
		col0.innerHTML = "&nbsp;";
		var col1 = nlinha.insertCell(1);
		col1.id="listapagamentoprocesso_"+pro;
		col1.colSpan=8;
		carregarListaPagamentoEmpenho('', pro);
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex+1);	
	}
}
</script>
<form method="post" name="formulario" id="formulario">
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
                                <tr>
                                        <td class="SubTituloDireita" width="30%">Preid:</td>
                                        <td><?php echo campo_texto( 'preid', 'N', 'S', 'Preid', 10, 200, '##########', ''); ?></td>
                                </tr>
                                <tr>
                                        <td class="SubTituloDireita">Obrid:</td>
                                        <td><?php echo campo_texto( 'obrid', 'N', 'S', 'Obrid', 10, 200, '##########', ''); ?></td>
                                </tr>
				<tr>
					<td class="SubTituloDireita">Número de Processo:</td>
					<td>
					<?php
						$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
						$numeroprocesso = $filtro;
						echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
					?>
					</td>
				</tr>								
				<tr>
					<td class="SubTituloDireita">Município:</td>
					<td>
						<?php 
							$filtro = simec_htmlentities( $_POST['municipio'] );
							$municipio = $filtro;
							echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Estado:</td>
					<td>
						<?php
							$estuf = $_POST['estuf'];
							$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
							$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Resolução:</td>
					<td>
						<?php 
							$resid = $_POST['resid'];
							$sql = "select resid as codigo, resdescricao as descricao from par.resolucao where resstatus='A'";
							$db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Esfera:</td>
					<td>
						<?php 
							$esfera = $_POST['esfera'];
							$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
							$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' ); 
						?>
					</td>
				</tr>
				<tr>
				<td class="SubTituloDireita">Usuário de Criação:</td>
					<td>
						<?php 
							$usuario = $_POST['usuario'];
							$sql = "SELECT DISTINCT u.usucpf as codigo, u.usunome as descricao FROM seguranca.usuario u INNER JOIN par.empenho emp ON emp.usucpf = u.usucpf and empstatus = 'A' ORDER BY u.usunome";
							$db->monta_combo( "usuario", $sql, 'S', 'Todas os usuários', '', '' ); 
						?>
					</td>
				</tr>

				<tr>
					<td class="SubTituloDireita">Tipo de Obra:</td>
					<td>
						<input <?php echo $_REQUEST['tipoobra'] == "P" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="P" />Proinfancia
						<input <?php echo $_REQUEST['tipoobra'] == "Q" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="Q" />Quadra
						<?if(empty($_REQUEST['tipoobra'])){ ?>
							<input <?php echo $_REQUEST['tipoobra'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="T" checked="checked" />Todos
						<?}else{ ?>
							<input <?php echo $_REQUEST['tipoobra'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="T" />Todos
						<?} ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Empenho:</td>
					<td>
						<input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" />Empenho Solicitado
						<input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" />Empenho Não Solicitado
						<?if(empty($_REQUEST['empenhado'])){ ?>
							<input <?php echo $_REQUEST['empenhado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="3" checked="checked" />Todos
						<?}else{ ?>
							<input <?php echo $_REQUEST['empenhado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="3" />Todos
						<?} ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Termo:</td>
					<td>
						<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="1" />Termo Gerado
						<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="2" />Termo Não Gerado
						<?if(empty($_REQUEST['termogerado'])){ ?>
							<input <?php echo $_REQUEST['termogerado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="3" checked="checked" />Todos
						<?}else{ ?>
							<input <?php echo $_REQUEST['termogerado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="3" />Todos
						<?} ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Assinatura:</td>
					<td>
						<input <?php echo $_REQUEST['assinado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="1" />Assinado
						<input <?php echo $_REQUEST['assinado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" />Não Assinado
						<?if(empty($_REQUEST['assinado'])){ ?>
							<input <?php echo $_REQUEST['assinado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="3" checked="checked" />Todos
						<?}else{ ?>
							<input <?php echo $_REQUEST['assinado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="3" />Todos
						<?} ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Pagamento:</td>
					<td>
						<input <?php echo $_REQUEST['pagamento'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="1" />Pagamento Solicitado
						<input <?php echo $_REQUEST['pagamento'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="2" />Pagamento Não Solicitado
						<input <?php echo $_REQUEST['pagamento'] == "3" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="3" />Pagamento efetivado
						<?if(empty($_REQUEST['pagamento'])){ ?>
							<input <?php echo $_REQUEST['pagamento'] == "0" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="0" checked="checked" />Todos
						<?}else{ ?>
							<input <?php echo $_REQUEST['pagamento'] == "0" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="0"  />Todos
						<?} ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Metodologia Inovadora:</td>
					<td>
						<input <?php echo $_REQUEST['metodologia'] == "1" ? "checked='checked'" : "" ?> type="checkbox" name="metodologia" value="1" />
					</td>
				</tr>
				<tr>
					<td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')">
						<b>Possui Obras com:</b>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Ordem de serviço inserida:</td>
					<td>
						<input type="radio" value="S" id="homologacao" name="homologacao" <? if($_REQUEST["homologacao"] == "S") { echo "checked"; } ?> /> Sim
						<input type="radio" value="N" id="homologacao" name="homologacao" <? if($_REQUEST["homologacao"] == "N") { echo "checked"; } ?> /> Não Validados
						<input type="radio" value="P" id="homologacao" name="homologacao"  <? if($_REQUEST["homologacao"] == "P") { echo "checked"; } ?> /> Não Analisados
						<input type="radio" value="" id="homologacao" name="homologacao"  <? if($_REQUEST["homologacao"] == "") { echo "checked"; } ?> /> Todos
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Execução 25% Validada:</td>
					<td>
						<input type="radio" value="S" id="execucao25" name="execucao25" <? if($_REQUEST["execucao25"] == "S") { echo "checked"; } ?> /> Sim
						<input type="radio" value="N" id="execucao25" name="execucao25" <? if($_REQUEST["execucao25"] == "N") { echo "checked"; } ?> /> Não Validados
						<input type="radio" value="P" id="execucao25" name="execucao25"  <? if($_REQUEST["execucao25"] == "P") { echo "checked"; } ?> /> Não Analisados
						<input type="radio" value="" id="execucao25" name="execucao25"  <? if($_REQUEST["execucao25"] == "") { echo "checked"; } ?> /> Todos
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Execução 50% Validada:</td>
					<td>
						<input type="radio" value="S" id="execucao50" name="execucao50" <? if($_REQUEST["execucao50"] == "S") { echo "checked"; } ?> /> Sim
						<input type="radio" value="N" id="execucao50" name="execucao50" <? if($_REQUEST["execucao50"] == "N") { echo "checked"; } ?> /> Não Validados
						<input type="radio" value="P" id="execucao50" name="execucao50"  <? if($_REQUEST["execucao50"] == "P") { echo "checked"; } ?> /> Não Analisados
						<input type="radio" value="" id="execucao50" name="execucao50"  <? if($_REQUEST["execucao50"] == "") { echo "checked"; } ?> /> Todos
					</td>
				</tr>
                <tr>
                    <td class="SubTituloDireita" >Situação da Obra:</td>
                    <td colspan="3">
                        <?php
                        $sql = "SELECT esdid as codigo, esddsc as descricao
                            from workflow.estadodocumento
                            where tpdid = 105
                            and estadodocumento.esdstatus = 'A'
                            order by descricao";

                        $esdid = $_REQUEST['esdid'];
                        $db->monta_combo( "esdid", $sql, 'S', 'Selecione', '', '', '', 200, 'N', 'esdid' );
                        ?>
                    </td>
                </tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Obras não paga até:</td>
					<td>
						<?php $perc_n_exec = $_REQUEST['perc_n_exec']; ?>
						<?php echo campo_texto( 'perc_n_exec', 'N', 'S', '', 3, 3, '[#]', '', 'right', '', 0, ''); ?> %
					</td>
				</tr>
                <tr>
                    <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
                    <td>
                        <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
                        <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
                        <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
                    <td>
                        <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
                        <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
                        <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Possui Solicitação de Desembolso:</td>
                    <td>
                        <input <?php echo $_REQUEST['boSolicitacaoDesembolso'] == "S" ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" id="boSolicitacaoDesembolso_S" value="S" >Sim
                        <input <?php echo $_REQUEST['boSolicitacaoDesembolso'] == "N" ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" id="boSolicitacaoDesembolso_N" value="N" >Não
                        <input <?php echo empty($_REQUEST['boSolicitacaoDesembolso']) ? "checked='checked'" : "" ?> type="radio" name="boSolicitacaoDesembolso" value=""   />Todos
                    </td>
                </tr>
			    <?php if( $db->testa_superuser() ){?>
			    <tr>
			        <td class="SubTituloDireita" style="color:red">Possui erro na distribuição do pagamento:</td>
			        <td>
			            <input <?php echo $_REQUEST['pagamentoMaiorQueEmpenho'] == "S" ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" id="pagamentoMaiorQueEmpenho" value="S" >Sim
			            <input <?php echo empty($_REQUEST['pagamentoMaiorQueEmpenho']) ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" value=""   />Todos
			        </td>
			    </tr>
			    <?php }?>
				<tr>
					<td class="SubTituloDireita"></td>
					<td>
						<input type="submit" name="pesquisar" value="Pesquisar" />
						<input type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/pagamentoPAC&acao=A';" />
						<input type="submit" name="exportarexcel" value="Exportar Excel" />
					</td>
				</tr>
			</tbody>
		</table>
</form>
<?php
/*
$wheremun[] = "( p.muncod IS NOT NULL )";
$whereest[] = "( p.muncod IS NULL )";
*/

$wheremun[] = "( p.prostatus = 'A' )";
$whereest[] = "( p.prostatus = 'A' )";
$select = "";
$inner = '';
$innerEstadoMun = NULL;

if($_REQUEST['pesquisar'] || $_REQUEST['exportarexcel']) {

	if( $_REQUEST['numeroprocesso'] ){
		$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
		$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
		$wheremun[] = "p.pronumeroprocesso = '{$_REQUEST['numeroprocesso']}'";
		$whereest[] = "p.pronumeroprocesso = '{$_REQUEST['numeroprocesso']}'";		
	}	
	
	if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				//$whereest[] = "1=2";
				$whereest[] = "p.muncod is not null";
				$innerEstadoMun .= " INNER JOIN territorios.municipio m ON m.muncod = p.muncod ";
				
				break;
			case 'Estadual':
				//$wheremun[] = "1=2";
				$whereest[] = "p.muncod is null";
				$innerEstadoMun .= " LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
									 INNER JOIN territorios.estado e ON e.estuf = p.estuf ";
				break;
		}
	}
	
        
        // Divisão do Filtro Preid e Obrid
        $preIdObrIdClause = false;
        if($_REQUEST['preid']) {
            $preIdObrIdClause = true;
            $preIdObrIdClauseValue = $_REQUEST['preid'];
            $whereest[] = " pp.preid = '{$_REQUEST['preid']}' ";
            $inner .= " INNER JOIN par.processoobraspaccomposicao pp ON pp.proid = p.proid ";
            $wheremun[] = " pp.preid = '{$_REQUEST['preid']}' ";
        }

        if($_REQUEST['obrid']) {
            $preIdObrIdClause = true;
            $preIdObrIdClauseValue = $_REQUEST['obrid'];
            $whereest[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
            $wheremun[] = " obr.obrid = '{$_REQUEST['obrid']}' ";
            $inner .= "	INNER JOIN par.processoobraspaccomposicao pp ON pp.proid = p.proid
						INNER JOIN obras2.obras obr ON obr.preid = pp.preid AND obr.obrstatus = 'A' AND obr.obridpai is null ";
        }
	
	if($_REQUEST['municipio']) {
		$wheremun[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		//$whereest[] = "1=2";
	}
	
	if($_REQUEST['usuario']){
		$wheremun[] = "p.pronumeroprocesso in ( select empnumeroprocesso from par.empenho where usucpf = '".$_REQUEST['usuario']."' and empstatus = 'A')";
		$whereest[] = "p.pronumeroprocesso in ( select empnumeroprocesso from par.empenho where usucpf = '".$_REQUEST['usuario']."' and empstatus = 'A')";
	}
	
	if($_REQUEST['estuf']) {
		//$wheremun[] = "m.estuf='".$_REQUEST['estuf']."'";
		$whereest[] = "(p.estuf = '".$_REQUEST['estuf']."' OR m.estuf = '".$_REQUEST['estuf']."')";
	}
        
	if($_REQUEST['empenhado'] == "1") {
		 $sqlEmpenhoCancelado = " ( select sum(saldo) from par.vm_dadosempenhos  where processo = p.pronumeroprocesso having  sum(saldo) > 0  ) ";
		$wheremun[] = "{$sqlEmpenhoCancelado} > 0";
	//	$whereest[] = "{$sqlEmpenhoCancelado} > 0";
	}
	if($_REQUEST['empenhado'] == "2") {
		 $sqlEmpenhoCancelado = " ( select sum(saldo) from par.vm_dadosempenhos  where processo = p.pronumeroprocesso having  sum(saldo) > 0  ) ";
		$wheremun[] = "{$sqlEmpenhoCancelado} = 0";
	//	$whereest[] = "{$sqlEmpenhoCancelado} = 0";
	}
        
	if($_REQUEST['pagamento'] == "1") {
		$wheremun[] = "p.pronumeroprocesso IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
	//	$whereest[] = "p.pronumeroprocesso IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
	}
	if($_REQUEST['pagamento'] == "2") {
		$wheremun[] = "p.pronumeroprocesso NOT IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
	//	$whereest[] = "p.pronumeroprocesso NOT IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
	}
	if($_REQUEST['pagamento'] == "3") {
		$wheremun2[] = "valorpagamento > 0";
	//	$whereest2[] = "valorpagamento > 0";
	}
	
	if($_REQUEST['tipoobra'] == "P") {
		$wheremun[] = "protipo = 'P'";
		$whereest[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$wheremun[] = "protipo <> 'P'";
		$whereest[] = "protipo <> 'P'";
	}
	if($_REQUEST['resid']) {
		$wheremun[] = "r.resid = '".$_REQUEST['resid']."'";
		$whereest[] = "r.resid = '".$_REQUEST['resid']."'";
	}
	
	if( $_REQUEST['homologacao'] == 'S' 
		|| $_REQUEST['homologacao'] == 'N' 
		||  $_REQUEST['homologacao'] == 'P' 
		|| $_REQUEST['execucao25'] == 'S'
	 	|| $_REQUEST['execucao25'] == 'N'
	 	||  $_REQUEST['execucao25'] == 'P' 
	 	|| $_REQUEST['execucao50'] == 'S'
	 	|| $_REQUEST['execucao50'] == 'N'
	 	|| $_REQUEST['execucao50'] == 'P'
	 	|| $_REQUEST['esdid'] != ''
	 	|| $_REQUEST['perc_n_exec'] != ''
		){
		$inner .= " INNER JOIN par.processoobraspaccomposicao poc ON poc.proid = p.proid and poc.pocstatus = 'A'
					INNER JOIN obras.preobra pre ON pre.preid = poc.preid
					INNER JOIN obras2.obras obr ON obr.preid = pre.preid
					LEFT JOIN obras2.validacao val ON val.obrid = obr.obrid ";
	}
	
	if( $_REQUEST['homologacao'] == 'S' ) {
		$wheremun[] = " val.vldstatushomologacao = 'S' ";
		$whereest[] = " val.vldstatushomologacao = 'S' ";
	}
	elseif( $_REQUEST['homologacao'] == 'N' ){
	
		$wheremun[] = " (val.vldstatushomologacao = 'N') ";
		$whereest[] = " (val.vldstatushomologacao = 'N') ";
	}
	elseif( $_REQUEST['homologacao'] == 'P' ){

		$wheremun[] = " (val.vldstatushomologacao is null) ";
		$whereest[] = " (val.vldstatushomologacao is null) ";
	}
	
	if( $_REQUEST['execucao25'] == 'S' ){
		$wheremun[] = " val.vldstatus25exec = 'S' ";
		$whereest[] = " val.vldstatus25exec = 'S' ";
	}
	elseif( $_REQUEST['execucao25'] == 'N' ){
		$wheremun[] = " val.vldstatus25exec = 'N' ";
		$whereest[] = " val.vldstatus25exec = 'N' ";
	}
	elseif( $_REQUEST['execucao25'] == 'P' ){
		$wheremun[] = " val.vldstatus25exec IS NULL ";
		$whereest[] = " val.vldstatus25exec IS NULL ";
	}
	
	if( $_REQUEST['execucao50'] == 'S' ){
		$wheremun[] = " val.vldstatus50exec = 'S' ";
		$whereest[] = " val.vldstatus50exec = 'S' ";
	}
	elseif( $_REQUEST['execucao50'] == 'N' ){
		$wheremun[] = " val.vldstatus50exec = 'N' ";
		$whereest[] = " val.vldstatus50exec = 'N' ";
	}
	elseif( $_REQUEST['execucao50'] == 'P' ){
		$wheremun[] = " val.vldstatus50exec IS NULL ";
		$whereest[] = " val.vldstatus50exec IS NULL ";
	}
	
	if( $_REQUEST['perc_n_exec'] != '' ){
		
		$inner .= " INNER JOIN (	SELECT preid, SUM(pobpercentualpag) as perc_pag 
					FROM par.pagamentoobra pob
					INNER JOIN par.pagamento pag ON pag.pagid = pob.pagid and pag.pagstatus = 'A' 
					GROUP BY preid) as pag ON pag.preid = poc.preid ";
		
		$wheremun[] = " ( 100 - coalesce(pag.perc_pag,0) ) <= {$_REQUEST['perc_n_exec']} ";
		//$whereest[] = " ( 100 - coalesce(pag.perc_pag,0) ) <= {$_REQUEST['perc_n_exec']}";
	}
	
	if($_REQUEST['metodologia']) {
		$wheremun[] = "p.proid IN (SELECT poc.proid FROM par.processoobra po
	  				   INNER JOIN par.processoobraspaccomposicao poc ON poc.proid = po.proid and poc.pocstatus = 'A'
	  				   INNER JOIN obras.preobra p ON p.preid = poc.preid 
	  				   WHERE po.prostatus = 'A' and p.ptoid IN (42,43,44,45)) ";
	}	
	
	
	
	if($_REQUEST['termogerado'] != "3") {
		if($_REQUEST['termogerado'] == "1") {
		}
		if($_REQUEST['termogerado'] == "2") {
			$wheremun[] = " p.proid not in (select p.proid from par.termocompromissopac where  terstatus = 'A' and proid = p.proid )  ";
		}
	}
	
	if($_REQUEST['assinado'] != "3") {
		//$inner .= "INNER JOIN par.termocompromissopac t2 ON t2.proid = p.proid  AND t2.terstatus = 'A' ";
		if($_REQUEST['assinado'] == "1") {
			$wheremun[] = "t.terassinado = TRUE ";
			//$whereest[] = "t.terassinado = TRUE ";
		}
		if($_REQUEST['assinado'] == "2") {
			$wheremun[] = "( t.terassinado = FALSE OR t.terassinado IS NULL ) ";
		//	$whereest[] = "( t.terassinado = FALSE OR t.terassinado IS NULL )";
		}
	}

    $pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        // Negando estadual
      //  $whereest[] = "1=2";
        $wheremun[] = " p.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        // Negando estadual
      //  $whereest[] = "1=2";
        $wheremun[] = " p.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        // Negando municipal
      //  $wheremun[] = "1=2";
        $whereest[] = " p.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        // Negando municipal
      // $wheremun[] = "1=2";
        $whereest[] = " p.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }
}

if( $_REQUEST['esdid'] != '' ){
	
	$inner .= " INNER JOIN workflow.documento obrd ON obrd.docid = obr.docid ";
	
    $wheremun[] = " obrd.esdid = '{$_REQUEST['esdid']}' ";
}

if( $db->testa_superuser() && $_POST['pagamentoMaiorQueEmpenho'] == 'S' ){
	$_REQUEST['tooid'] = 1;
	$query = sql_verificaPagamentoMaiorQueEmpenhoObra();
	$wheremun[] = $whereest[] = "(SELECT
									COUNT(empid)
								FROM
									(
									".str_replace(Array('%processo%', '%filtro%'), Array('p.pronumeroprocesso',''), $query)."					
									) as foo ) > 0";
}

if($_REQUEST['exportarexcel']){
	$cabecalho = array("Nº Processo","Resolução","Município","UF", "Tipo obra","Valor obra(R$)","Valor empenhado(R$)","Valor pagamento(R$)", "Saldo da Conta", "Qtd obras no processo","Usuário Criação");
} else {
	$cabecalho = array("&nbsp;","&nbsp;","Nº Processo","Resolução","Município","UF", "Tipo obra","Valor obra(R$)","Valor empenhado(R$)","Valor pagamento(R$)","Qtd obras no processo");
}

$select = '';
$sqlMun = "SELECT DISTINCT * FROM (
		SELECT ";

if(!$_REQUEST['exportarexcel']){
	$select .= "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPagamento(\''||p.pronumeroprocesso||'\', this);\"></center>' as mais,";
	if( $preIdObrIdClause ){
		$select .=	"'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamento&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'&preidobrid={$preIdObrIdClauseValue}\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes,";
	}else{
		$select .=	"'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoPagamento&acao=A&processo=' || p.pronumeroprocesso || '&proid='||p.proid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes,";
	}
} 
	
$sqlMun .=	"INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
			".(($wheremun)?"WHERE ".implode(" AND ", $wheremun):"")." ) foo 
			".(($wheremun2)?"WHERE ".implode(" AND ", $wheremun2):"");
		
if($_REQUEST['exportarexcel']){
 	$usucriacao .= ", array_to_string(array(SELECT u.usunome from seguranca.usuario u inner join par.empenho emp on emp.usucpf = u.usucpf  where emp.empnumeroprocesso = p.pronumeroprocesso and empstatus = 'A'), ', ') as usunome ";
}

if( $_REQUEST['termogerado'] == "1" || $_REQUEST['assinado'] != "3" ){
	$inner .= "INNER JOIN par.termocompromissopac t ON t.proid = p.proid  AND terstatus = 'A' ";
}

if($whereest){
	$monta = "and ";
}

// if($innerEstadoMun == NULL){
	$innerEstadoMun = "LEFT JOIN territorios.municipio m ON m.muncod = p.muncod  
						LEFT JOIN territorios.estado e ON e.estuf = p.estuf ";
// }

if( !empty($_REQUEST['boSolicitacaoDesembolso']) ){
	$fil = '';
	if( $_REQUEST['boSolicitacaoDesembolso'] == 'S' ){
		$fil = 'p.proid in';
	} 
	if( $_REQUEST['boSolicitacaoDesembolso'] == 'N' ){
		$fil = 'p.proid not in';		
	}
	$wheremun[] = " $fil (SELECT
										proid /*,
									    preid,
										sldpercpagamento,
									    perc,
									    perc_pago*/
									FROM(
									    SELECT 
									        pp.proid,
									        pp.preid,
									        sd.sldpercpagamento,
									        (SELECT obrpercentultvistoria FROM obras2.obras 
									            WHERE preid = pre.preid AND obrstatus = 'A' AND obridpai IS NULL) as perc,
									        ((SELECT SUM(pobvalorpagamento) 
									        FROM par.pagamentoobra p2 
									            inner join par.pagamento pag2 ON pag2.pagid = p2.pagid
									        WHERE p2.preid = pre.preid
									            and pag2.pagstatus = 'A' 
									            and pag2.pagsituacaopagamento not ilike '%CANCELADO%' ) / pre.prevalorobra)*100 as perc_pago
									    FROM 
									        par.processoobraspaccomposicao pp 
									        inner join obras.preobra pre on pre.preid = pp.preid and pre.prestatus = 'A'
									        inner join obras2.solicitacao_desembolso sd on sd.obrid = pre.obrid and sd.sldstatus = 'A'
									        inner join workflow.documento d ON d.docid = sd.docid
									    WHERE
									        pp.pocstatus = 'A'
									        and d.esdid = 1576
									) foo
									WHERE (perc - perc_pago) >= sldpercpagamento )";
}

if($_REQUEST['exportarexcel']) {

	$saldo_bancario = " par.retornasaldoprocesso(p.pronumeroprocesso) as saldobancario,";
	$ProcessoN 		= "
                    		substring(p.pronumeroprocesso  from 1 for 5)||'.'||
					   		substring(p.pronumeroprocesso from 6 for 6)||'/'||
					   		substring(p.pronumeroprocesso from 12 for 4)||'-'||
					   		substring(p.pronumeroprocesso from 16 for 2)
                    		
                    		as processo, ";
}
else
{
	$ProcessoN 		= "'<span class=\"processo_detalhe\" >' || p.pronumeroprocesso || '<\span>' as processo, ";
}

$sql = "
	SELECT DISTINCT ".$select."
		{$ProcessoN}
		r.resdescricao as resolucao,
		m.mundescricao as municipio, 
		coalesce(p.estuf, m.estuf) as uf, 
		CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
		( 	SELECT ROUND(SUM(pr.prevalorobra),2)
			FROM obras.preobra pr
			INNER JOIN par.processoobraspaccomposicao pp ON pr.preid = pp.preid
			WHERE 	pp.pocstatus = 'A'
			AND proid = p.proid )
		 AS valortotal,
		(SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE processo = p.pronumeroprocesso) as valorempenhado,
		-- VALOR PAGO DO PROCESSO
		(
		SELECT COALESCE(SUM(pg.pagvalorparcela),0.00) 
		FROM par.pagamento pg 
		INNER JOIN par.empenho em ON em.empid = pg.empid
		WHERE 	em.empstatus = 'A'
			AND pg.pagstatus='A'
			AND em.empnumeroprocesso = p.pronumeroprocesso
		) AS valorpago,
		$saldo_bancario
		-- QUANTIDADE DE OBRAS NO PROCESSO
		(
			SELECT count(pp.preid)
			FROM obras.preobra po
			INNER JOIN par.processoobraspaccomposicao pp ON po.preid = pp.preid
			WHERE 	pp.pocstatus = 'A'
				AND pp.proid = p.proid
		) AS qtdobrasnoprocesso
		$usucriacao
	FROM par.processoobra p
	".$innerEstadoMun."
	LEFT JOIN par.resolucao r ON r.resid = p.resid
	".$inner."
	WHERE prostatus = 'A' AND
	".implode(' AND ' , $wheremun).$monta.implode(' AND ' , $whereest);

// dbg( simec_htmlentities( $sql),1);

    if($_REQUEST['exportarexcel']) {
        ob_clean();
        header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
        header ( "Pragma: no-cache" );
        header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
        header ( "Content-Disposition: attachment; filename=SIMEC_PagamentoObrasPAC".date("Ymdhis").".xls");
        header ( "Content-Description: MID Gera excel" );
        $db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
        exit;
    } else {
        // Só monta lista quando clicar em pesquisar
       
        if(isset($_REQUEST['estuf'])){
            $db->monta_lista($sql,$cabecalho,20,10,'N','center','N','formprocesso');
        }
    }
?>
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="hidden" name="ws_empid" id="ws_empid" value="" />
				<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>