<?php

if($_POST['habilita']){
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaHabilitaPar($_POST['cnpj']);
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaEmpenhoPorProcesso') {
	carregaEmpenhoPorProcesso($_REQUEST);
	exit();
}

if( $_REQUEST['requisicao'] == 'historicoCancelamento') {
	historicoCancelamento($_REQUEST);
	exit();
}
/* if($_REQUEST['requisicao']) {
	ver($_REQUEST,d);
	$_REQUEST['requisicao']($_REQUEST);
	exit;
} */

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_PAR );
monta_titulo( $titulo_modulo, $tabela_execucao );

?>
<style>
	#pesquisas thead tr td
	{
		background-color: #F7F7F7;
	}

	#pesquisas thead tr td.textosTabela
	{
		width: 200px;
	}
	#pesquisas thead tr td.camposTabela
	{
		width: 300px;
	}
	#pesquisas tbody tr td
	{
		background-color: #F7F7F7;
	}
	
	#pesquisas tbody tr td.SubTituloDireita
	{
		FONT-SIZE: 8pt;
    	COLOR: black;
    	FONT-FAMILY: Arial, Verdana;
    	TEXT-ALIGN: right;
		BACKGROUND-COLOR: #dcdcdc;
	}
	
	#pesquisas tbody tr td input.botao
	{
		border: 1px solid #757575;
		background-color: #DCDCDC;
		font-size: 11px;
		font-family: arial;
		font-weight: bold;
	}
	
	
	
	
</style>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/par.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />

<script type="text/javascript">
	jQuery.noConflict();
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,140);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}
	
	function verificaHabilita(cnpj){
	
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/empenhoPar&acao=A",
	   		data: "habilita=true&cnpj=" + cnpj,
	   		async: false,
	   		success: function(msg){
	   			alert('Dados Atualizado com Sucesso!');
	   			window.location.href = window.location;
				//jQuery('#debug').html(msg);
	   		}
   		});
	}
	function submeteFormularioExcel(){
		selectAllOptions( formulario.grupomunicipio );
		jQuery('#requisicao').val('exportarexcel');
		jQuery('#formulario').submit();
	}
	function submeteFormulario(){
		selectAllOptions( formulario.grupomunicipio );
		jQuery('#requisicao').val('pesquisar');
		jQuery('#formulario').submit();
	}

	function telaLogin( action ) {
    	jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
    	
    	/*jQuery.ajax({
    		type: "POST",
    		//url: window.location.href,
    		async: false,
    		success: function(msg){*/
    			jQuery( "#div_login" ).show();
    			//jQuery( "#mostra_dialog" ).html(msg);
    			jQuery( '#div_login' ).dialog({
    				resizable: false,
    				width: 500,
    				modal: true,
    				show: { effect: 'drop', direction: "up" },
    				buttons: {
    					'Ok': function() {
    						if( action == 'consultar' ){
    							consultarEmpenhoWS();
    						}
    						if( action == 'cancelar' ){
    							cancelarEmpenhoWS();
    						}
    					},
    					'Fechar': function() {
    						jQuery( this ).dialog( 'close' );
    					}
    				}
    			});
    		/*}
    	});*/
    }

	function cancelarEmpenhoWS() {

		var wsusuario = document.getElementById('wsusuario').value;
		var wssenha = document.getElementById('wssenha').value;
		var wsempid = document.getElementById('wsempid').value;
		var wsprocesso = document.getElementById('processo').value;
		var wsespecie = document.getElementById('ws_especie').value;
		
		if(!wsusuario){
			alert('Favor informar o nome de usuário!');
			return false;
		}
		if(!wssenha){
			alert('Favor informar a senha!');
			return false;
		}
		
		//document.getElementById('div_auth_cancela').style.display = 'none';
		
		divCarregando();
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
	   		data: "requisicao=cancelarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha + "&wsespecie="+wsespecie,
	   		async: false,
	   		success: function(msg){
	   		//document.getElementById('debug').innerHTML = msg;
	   		alert(msg);}
		});
		
		//carregarListaEmpenhoProcesso( wsprocesso );
		
		divCarregado();
		window.location.reload();
		
	}
	
	function cancelarEmpenho(empid, processo, especie) {
	
		/* document.getElementById('ws_usuario_consulta').value;
		document.getElementById('ws_senha_consulta').value;
		document.getElementById('ws_empid').value = empid;
		document.getElementById('ws_processo').value = processo;
		document.getElementById('ws_especie').value = especie;
		
		document.getElementById('div_auth_cancela').style.display = 'block';
		document.body.scrollTop = 0; */

		document.getElementById('wsempid').value = empid;
		document.getElementById('ws_especie').value = especie;
		telaLogin( 'cancelar' );
	}
	
	function reduzirEmpenho(empid, processo, especie) {
		window.open('par.php?modulo=principal/reduzirEmpenho&acao=A&empid='+empid+'&processo='+processo+'&especie='+especie, 
        'reduzirEmpenho', 
        "height=400,width=800,scrollbars=yes,top=0,left=0" );
	}
	
	function consultarEmpenho(empid,processo) {
	
		/* document.getElementById('ws_usuario_consulta').value;
		document.getElementById('ws_senha_consulta').value;
		document.getElementById('ws_empid').value = empid;
		document.getElementById('ws_processo').value = processo;
		
		document.getElementById('div_auth_consulta').style.display = 'block';
		document.body.scrollTop = 0; */
		document.getElementById('wsempid').value = empid;
		document.getElementById('processo').value = processo;
		telaLogin( 'consultar' );
	}
	
	
	function consultarEmpenhoWS() {
		
		var wsusuario = document.getElementById('wsusuario').value;
		var wssenha = document.getElementById('wssenha').value;
		var wsempid = document.getElementById('wsempid').value;
		var wsprocesso = document.getElementById('processo').value;
		
		if(!wsusuario){
			alert('Favor informar o nome de usuário!');
			return false;
		}
		if(!wssenha){
			alert('Favor informar a senha!');
			return false;
		}
		
		//document.getElementById('div_auth_consulta').style.display = 'none';
		
		divCarregando();
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A",
	   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
	   		async: false,
	   		success: function(msg){
	   			//document.getElementById('debug').innerHTML = msg;
	   			alert(msg);
	   		}
		});
		
		//carregarListaEmpenhoProcesso( wsprocesso );
		//window.location.href = window.location;
		divCarregado();
		window.location.reload();
		
	}
</script>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="processo" id="processo" value="<?php echo $_REQUEST['processo']?>" />
	<input type="hidden" name="wsempid" id="wsempid" value="" />
	<input type="hidden" name="ws_especie" id="ws_especie" value="" />
<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	<tr>
		<td class="SubTituloDireita">Número de Processo:</td>
		<td colspan="3">
		<?php
			$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
			$numeroprocesso = $filtro;
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
		?>
		</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php 
			$filtro = simec_htmlentities( $_POST['municipio'] );
			$municipio = $filtro;
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Esfera:</td>
		<td colspan="3">
			<?php 
				$esfera = $_POST['esfera'];
				$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
				$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' ); 
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Ano do Processo:</td>
		<td colspan="3">
			<?php
			$anoprocesso = $_POST['anoprocesso'];
			$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao from par.processopar where substring(prpnumeroprocesso,12,4) <> '' and prpstatus = 'A' ";
			$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Forma de Execução:</td>
		<td colspan="3">
		<?php
			$frmid = $_POST['frmid'];
			$sql = "SELECT frmid as codigo , frmdsc as descricao FROM par.formaexecucao WHERE frmid in (6, 13) ORDER BY frmdsc ASC";
			$db->monta_combo( "frmid", $sql, 'S', 'Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Empenho:</td>
		<td colspan="3">
			<?
			$empenhado = $_REQUEST['empenhado'];
			$arrEmpenho = array(
								array('codigo' => 1, 'descricao' => 'Empenhado Solicitado 100%'),
								array('codigo' => 2, 'descricao' => 'Nenhum Empenho Solicitado'),
								array('codigo' => 3, 'descricao' => 'Empenho Parcial (Subação)'),
								array('codigo' => 4, 'descricao' => 'Empenho Parcial (Processo)')
							);
			$db->monta_combo( "empenhado", $arrEmpenho, 'S', 'Selecione', '', '' );
			?>
			<!--  Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
			Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >-->
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo de Subação:</td>
		<td colspan="3">
		<?php
			$ptsid = $_POST['ptsid'];
			$sql = "SELECT pts.ptsid as codigo, pts.ptsdescricao || ' - ' || fe.frmdsc  AS descricao 
					FROM par.propostatiposubacao pts
					INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
					WHERE pts.ptsstatus = 'A' 
					AND fe.frmid in (6, 13)
					ORDER BY pts.ptsdescricao, fe.frmdsc";
			$db->monta_combo( "ptsid", $sql, 'S', 'Tipo de subação - Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Grupo de Município:</td>
		<td>
			<?php 

			$sql = "SELECT 
							tpmid as codigo, 
							tpmdsc as descricao
						FROM 
							territorios.tipomunicipio
						WHERE
							tpmstatus = 'A' 
							AND gtmid in ( 1,2 )
						ORDER BY
							descricao";
			
			if( $_REQUEST['grupomunicipio'][0] ){
				$sqlD = "SELECT 
							tpmid as codigo, 
							tpmdsc as descricao
						FROM 
							territorios.tipomunicipio
						WHERE
							tpmstatus = 'A' 
							AND gtmid in ( 1,2 )
							and tpmid in (". implode(",",$_REQUEST['grupomunicipio']). ")
						ORDER BY
							descricao";
				
				$grupomunicipio = $db->carregar($sqlD);
			}
			
			combo_popup('grupomunicipio', $sql, 'Selecione...', "400x400", 0, array(), '', 'S', false, false, 3 );
			?>															
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo Execução:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['tipoExecucao'] == "C" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="C" />Convênio 
			<input <?php echo $_REQUEST['tipoExecucao'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="T" />Termo
			<input <?php echo empty($_REQUEST['tipoExecucao']) ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="" />Todos
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita">Reformulação:</td>
		<td colspan="3">
		<?php 
			$aryReformulacao = array(
				array('codigo' => 't', 'descricao' => 'Termos em Reformulação'),
				array('codigo' => 'f', 'descricao' => 'Termos Reformulados')
			);
			$reformulacao = $_POST['reformulacao'];
			$db->monta_combo("reformulacao",$aryReformulacao, 'S', 'Todos', '', '', '',400,'N','reformulacao');	?>
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
<?php	if( $db->testa_superuser() ){?>
       <tr>
       		<td class="SubTituloDireita" style="color:red">Empenhos com Divergências:</td>
            <td>
            	<input <?php echo $_REQUEST['empenhodivergente'] == "S" ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value="S" >Sim
                <input <?php echo $_REQUEST['empenhodivergente'] == "N" ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value="N" >Não
                <input <?php echo empty($_REQUEST['empenhodivergente']) ? "checked='checked'" : "" ?> type="radio" name="empenhodivergente" value="" />Todos
           </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="color:red">Processos com valor empenhado superior ao valor da subação:</td>
            <td>
            	<input <?php echo $_REQUEST['vrlempenhomaior'] == "S" ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="S" >Sim
                <input <?php echo $_REQUEST['vrlempenhomaior'] == "N" ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="N" >Não
                <input <?php echo empty($_REQUEST['vrlempenhomaior']) ? "checked='checked'" : "" ?> type="radio" name="vrlempenhomaior" value="" />Todos
            </td>
       </tr>
<?php }?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td colspan="3">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input class="botao" type="button" name="pesquisar" id= "pesquisar" value="Pesquisar" onclick="submeteFormulario();">
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/empenhoPar&acao=A';" />
			<input class="botao" type="button" name="excel" value="Exportar Excel" onclick="submeteFormularioExcel();" />
		</td>
	</tr>
	</tbody>
</table>
</form>

<div id="div_login" title="Tela de Identificação" style="display: none; text-align: center;">
	<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario", "S", "S", "Usuário", "25","","","","","","", "id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" 
					onmouseover="MouseOver(this);" value="" size="26" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
	</table>
</div>
		
<div id="div_dialog" title="Histórico de Empenho" style="display: none; text-align: center;">
	<div style="padding:5px;text-align:justify;" id="mostra_dialog"></div>
</div>
<?php

	if($_REQUEST['requisicao'] == 'pesquisar' || $_REQUEST['requisicao'] == 'exportarexcel') {
   
		if($_REQUEST['empenhado'] == "3" && $_POST['estuf'] == ""){
			echo "<script language='javascript'>alert('Por favor, selecione um Estado!');history.go(-1);</script>";
			exit;
		}
		if($_REQUEST['numeroprocesso']) {
			$_REQUEST['numeroprocesso'] = str_replace(".","", $_REQUEST['numeroprocesso']);
			$_REQUEST['numeroprocesso'] = str_replace("/","", $_REQUEST['numeroprocesso']);
			$_REQUEST['numeroprocesso'] = str_replace("-","", $_REQUEST['numeroprocesso']);
			$where[] = "p.prpnumeroprocesso = '".$_REQUEST['numeroprocesso']."'";
		}
		if($_REQUEST['municipio']) {
			$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
		}
		
		if($_REQUEST['estuf']) {
			$where[] = "(iu.estuf='".$_REQUEST['estuf']."' OR iu.mun_estuf='".$_REQUEST['estuf']."')";
		}
		
		if($_REQUEST['frmid']) {
			$where[] = "p.prpid in (select ppc.prpid from par.processoparcomposicao ppc
			                            inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			                            inner join par.subacao sub on sub.sbaid = sd.sbaid
			                        where ppc.prpid = p.prpid AND ppc.ppcstatus = 'A' and sub.frmid = {$_REQUEST['frmid']})";
		}
		
		if($_REQUEST['ptsid']) {
			$where[] = "p.prpid in (select ppc.prpid from par.processoparcomposicao ppc
			                            inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
			                            inner join par.subacao sub on sub.sbaid = sd.sbaid
			                        where ppc.prpid = p.prpid AND ppc.ppcstatus = 'A' and sub.ptsid = {$_REQUEST['ptsid']})";
		}
		
		if($_REQUEST['empenhado'] == "1") {
			$where[] = " 1 = (SELECT 	case when pp.prptipoexecucao = 'C' then
								            -- CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
								             CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(e.empid) * 99) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
										else
								          --  CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
								          CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(e.empid) * 100) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
								       	end AS empenho
							            
							FROM par.processoparcomposicao pc
							INNER JOIN par.processopar pp on pp.prpid = pc.prpid
							INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
							LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid 
							WHERE pc.ppcstatus = 'A' and pp.prpnumeroprocesso = p.prpnumeroprocesso
							GROUP BY pp.prpid, pp.prptipoexecucao)";
		}
		if($_REQUEST['empenhado'] == "2") {
			$where[] = " p.prpnumeroprocesso not in ( select empnumeroprocesso from par.empenho where empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' )
							/*2 = (SELECT 	case when pp.prptipoexecucao = 'C' then
								            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
										else 
								            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
								        end AS empenho
							            
							FROM par.processoparcomposicao pc
							INNER JOIN par.processopar pp on pp.prpid = pc.prpid
							INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
							LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid 
							WHERE pc.ppcstatus = 'A' and pp.prpnumeroprocesso = p.prpnumeroprocesso
							GROUP BY pp.prpid, pp.prptipoexecucao) */"; 
		}
		if($_REQUEST['empenhado'] == "4"){
			$where[] = "p.prpid in (select 
										distinct pc2.prpid
									from 
										par.processoparcomposicao pc2
									inner join par.processopar prp2 on prp2.prpid = pc2.prpid
									inner join par.subacaodetalhe sd2 on sd2.sbdid = pc2.sbdid
									where 
										pc2.ppcstatus = 'A' and
										pc2.prpid = p.prpid AND
										(sd2.sbaid, sd2.sbdano) NOT IN ( SELECT sbaid, eobano FROM par.empenhosubacao where eobstatus = 'A' )
										AND prp2.prpnumeroprocesso in ( select empnumeroprocesso from par.empenho where empstatus = 'A' ))";
		}
		if($_REQUEST['empenhado'] == "3"){
		/*
			$where[] = " 3 = (SELECT 	case when pp.prptipoexecucao = 'C' then
											CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
										else 
								            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
								            ELSE
								                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL 
								                        ELSE  2 --NÃO FOI EMPENHADO
								                        END
								            END
								        end AS empenho
							            
							FROM par.processoparcomposicao pc
							INNER JOIN par.processopar pp on pp.prpid = pc.prpid
							INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
							LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO'
							LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid 
							WHERE pc.ppcstatus = 'A' and pp.prpnumeroprocesso = p.prpnumeroprocesso
							GROUP BY pp.prpid, pp.prptipoexecucao) ";
		*/ 
			$where[] = " 3 IN (select 
								CASE WHEN ref.vrlreforco IS NOT NULL THEN 
									CASE WHEN sum(es.eobvalorempenho+ref.vrlreforco) < (SELECT par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano))::numeric(20,2) THEN 3 --EMPENHADO PARCIAL 
									END
								ELSE
									CASE WHEN sum(es.eobvalorempenho) < (SELECT par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano))::numeric(20,2) THEN 3 --EMPENHADO PARCIAL 
									END
								END AS empenho
							FROM
								par.processopar prp
							INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
							INNER JOIN par.empenhosubacao es ON es.empid = emp.empid and eobstatus = 'A'
							left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
										from par.empenho
										where empcodigoespecie in ('02') and empstatus = 'A'
										group by 
											empnumeroprocesso,
											empcodigoespecie,
											empidpai) as ref on ref.empidpai = es.empid 
							WHERE
								prp.prpnumeroprocesso = p.prpnumeroprocesso  
							GROUP BY
								es.sbaid, es.eobano, ref.vrlreforco)";
		}  

		if($_REQUEST['tipoExecucao'] == "C") {
			$where[] = "p.prptipoexecucao = 'C'";
		}
		
		if($_REQUEST['tipoExecucao'] == "T") {
			$where[] = "p.prptipoexecucao = 'T'";
		}
		
		if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				$where[] = "iu.itrid = 2";
				break;
			case 'Estadual':
				$where[] = "CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END AND ( p.muncod IS NULL or p.muncod = '')";
				break;
		}
	}
	
	if($_REQUEST['grupomunicipio'][0]){
		$where[] = "tm.tpmid in (". implode(",",$_REQUEST['grupomunicipio']). ")";
		$joinGrupoMun = "LEFT JOIN territorios.muntipomunicipio mt ON mt.muncod = m.muncod
						 LEFT JOIN territorios.tipomunicipio tm ON tm.tpmid = mt.tpmid";
	}
	
	
//		if($_REQUEST['esfera'] == "e") {
//			$where[] = " CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END AND ( p.muncod IS NULL or p.muncod = '') ";
//		} elseif($_REQUEST['esfera'] == "m") {
//			$where[] = "iu.itrid = 2";
//		} else {
//			$where[] = "iu.itrid IN (1,2)";			
//		}

		if( $_REQUEST['dopusucpfvalidacaofnde'] != "" || $_REQUEST['dopusucpfvalidacaogestor'] != "" ){
			$inner = "LEFT JOIN par.vm_documentopar_ativos dp ON dp.prpid = p.prpid";
		}

		if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NOT NULL ";
		}
		if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaofnde IS NULL ";
		}
		if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NOT NULL ";
			if($_REQUEST['tipoExecucao'] == "T") {
				$where[] = "mdoid = 3"; // Termo
			}else if ($_REQUEST['tipoExecucao'] == "C"){
				$where[] = "mdoid = 2"; // Convênio
			}else if ($_REQUEST['tipoExecucao'] == "TTE"){
				$where[] = "mdoid in (2,3)"; // Convênio
			}
			
		}
		if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
			$where[] = "dp.dopusucpfvalidacaogestor IS NULL ";
			if($_REQUEST['tipoExecucao'] == "T") {
				$where[] = "mdoid = 3"; // Termo
			}else if ($_REQUEST['tipoExecucao'] == "C"){
				$where[] = "mdoid = 2"; // Convênio
			}else if ($_REQUEST['tipoExecucao'] == "TTE"){
				$where[] = "mdoid in (2,3)"; // Convênio
			}
			
		}

		if($_REQUEST['anoprocesso']){
			$condicaoProcessoPar = " and   substring(prpnumeroprocesso,12,4) = '".$_REQUEST['anoprocesso']."'";
		}

        $pendenciaObras = $_REQUEST['pendenciaObras'];
        if($pendenciaObras == '1'){ // com pendencias
            $where[] = " iu.itrid = '2' ";
            $where[] = " iu.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
        }

        if($pendenciaObras == '2'){ // sem pendencias
            $where[] = " iu.itrid = '2' ";
            $where[] = " iu.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
        }

        $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
        if($pendenciaObrasUF == '1'){ // com pendencias
            $where[] = " iu.itrid = '1' ";
            $where[] = " iu.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
        }

        if($pendenciaObrasUF == '2'){ // sem pendencias
            $where[] = " iu.itrid = '1' ";
            $where[] = " iu.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
        }

	}

	//$cabecalho = array("&nbsp;","&nbsp;","Nº Processo", "Município","UF", "Valor empenhado(R$)","Tipo");
	/*
	$sql = "SELECT DISTINCT
				'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPar(\''||p.prpnumeroprocesso||'\', this);\"></center>' as mais,
			   	'<center><img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A&processo=' || p.prpnumeroprocesso || '&prpid='||p.prpid||'&inuid='||p.inuid||'\',\'Empenho\',\'scrollbars=yes,height=500,width=800,status=no,toolbar=no,menubar=no,location=no\');\"></center>' as acoes, 
			   	p.prpnumeroprocesso, 
			   	m.mundescricao, 
			   	m.estuf, 
			 --  (SELECT sum(par.retornavrlempenhado(ep.empid)) FROM par.empenho ep WHERE ep.empnumeroprocesso=p.prpnumeroprocesso) as valorempenhado,
			 0 as valorempenhado,
			   -- (SELECT COUNT(emp.sbaid) FROM par.empenho eo INNER JOIN par.empenhosubacao emp ON emp.empid=eo.empid WHERE eo.empnumeroprocesso=p.prpnumeroprocesso) as qtdsubacaoempenhada
			   CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END AS tipo
		FROM par.instrumentounidade iu
		INNER JOIN par.pontuacao 		po   on po.inuid   = iu.inuid
		INNER JOIN par.acao 			a   on a.ptoid   = po.ptoid
		INNER JOIN par.subacao			s   on s.aciid   = a.aciid 
		INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = date_part('year', now())
		INNER JOIN par.processopar p ON p.inuid = iu.inuid
		LEFT JOIN par.empenhosubacao ep ON ep.sbaid = s.sbaid AND sd.sbdano = sd.sbdano
		LEFT JOIN par.propostatiposubacao  pts on pts.frmid = s.frmid
		LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
		".(($where)?"WHERE ".implode(" AND ", $where):"") 
		." and p.prptipoexecucao = 'C' ORDER BY m.estuf, m.mundescricao";
	dbg($sql);
	*/
	
	$colunaDivergente =  '';
	$filtroDivergente = '';
	$joinDivergente = '';
	
	if( $_REQUEST['empenhodivergente'] == 'S' ){
		$colunaDivergente 	= ' coalesce(emp.temdivergente, 0) as temdivergente, ';
// 		$filtroDivergente 	= ' where temdivergente > 0';
		$where[] 			= " temdivergente > 0 ";
		$joinDivergente 	= "	left join(
		                      	select
		                          	count(empenho) as temdivergente, processo
		                      	from (
		                          	select
								    	coalesce(e.empvalorempenho, 0) as vrlempenho,
								        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
								        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
								        e.empid as empenho,
								        e.empnumeroprocesso as processo,
								        e.empnumero as notaempenho,
								        e.empprotocolo as sequencial,
								        e.empcodigoespecie as especie,
								        'PAR' as tipo
								    from
								        par.processopar pp
								        inner join par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
								        left join par.empenhosubacao es on es.empid = e.empid and es.eobstatus = 'A'
								    where
								        pp.prpstatus = 'A'
								        and e.empstatus = 'A'
								        and e.empsituacao <> 'CANCELADO'
								    group by
								        e.empid,
								        e.empnumeroprocesso,
								        e.empnumero,
								        e.empcodigoespecie,
								        e.empprotocolo,
								        e.empvalorempenho,
								        e.empvalorempenho
		                         	) as foo
		                         	where
			                   			vrlempenho <> vrlempcomposicao
										and diferenca > 1
		                         	group by processo
		                  		) emp on emp.processo = p.prpnumeroprocesso";
	}
	if( $_REQUEST['empenhodivergente'] == 'N' ){
		$colunaDivergente 	= ' coalesce(emp.temdivergente, 0) as temdivergente, ';
// 		$filtroDivergente 	= ' where temdivergente <= 0';
		$where[] 			= " temdivergente IS NULL ";
		$joinDivergente 	= " left join(
		                      	select
		                          	count(empenho) as temdivergente, processo
		                      	from (
		                          	select
								    	coalesce(e.empvalorempenho, 0) as vrlempenho,
								        coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
								        (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
								        e.empid as empenho,
								        e.empnumeroprocesso as processo,
								        e.empnumero as notaempenho,
								        e.empprotocolo as sequencial,
								        e.empcodigoespecie as especie,
								        'PAR' as tipo
								    from
								        par.processopar pp
								        inner join par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso
								        left join par.empenhosubacao es on es.empid = e.empid and es.eobstatus = 'A'
								    where
								        pp.prpstatus = 'A'
								        and e.empstatus = 'A'
								        and e.empsituacao <> 'CANCELADO'
								    group by
								        e.empid,
								        e.empnumeroprocesso,
								        e.empnumero,
								        e.empcodigoespecie,
								        e.empprotocolo,
								        e.empvalorempenho,
								        e.empvalorempenho
		                         	) as foo
		                         	where
			                   			vrlempenho <> vrlempcomposicao
										and diferenca > 1
		                         	group by processo
		                  		) emp on emp.processo = p.prpnumeroprocesso";
	}
	
	if( $_REQUEST['vrlempenhomaior'] == 'S' ){
		$where[] = "p.prpnumeroprocesso in (select 
										empnumeroprocesso 
									from(
									    select distinct
									        es.sbaid, es.eobano, e.empnumeroprocesso,
									        ((sum(es.eobvalorempenho) + sum(coalesce(ref.reevlr,0))) - sum(coalesce(can.vrlcancelado, 0))) as vrlempenho,
									        par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano) as vrlsubacao
									    from
									        par.empenho e 
									        inner join par.empenhosubacao es on es.empid = e.empid
									        left join (
									            select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as reevlr, ep.empcodigoespecie, es1.sbaid, es1.eobano
									            from par.empenho ep
									                inner join par.empenhosubacao es1 on es1.empid = ep.empid
									            where ep.empcodigoespecie in ('02')
									            group by 
									                ep.empnumeroprocesso,
									                ep.empcodigoespecie,
									                ep.empidpai, es1.sbaid, es1.eobano
									        ) as ref on ref.empidpai = e.empid and ref.sbaid = es.sbaid and ref.eobano = es.eobano
									        left join (
									            select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlcancelado, ep.empcodigoespecie, es1.sbaid, es1.eobano
									            from par.empenho ep
									                inner join par.empenhosubacao es1 on es1.empid = ep.empid
									            where ep.empcodigoespecie in ('03', '04', '13')
									            group by 
									                ep.empnumeroprocesso,
									                ep.empcodigoespecie,
									                ep.empidpai, es1.sbaid, es1.eobano
									        ) as can on can.empidpai = e.empid and can.sbaid = es.sbaid and can.eobano = es.eobano
									    where 
									        e.empcodigoespecie not in ('03', '04', '13', '02')
									    group by 
									        es.sbaid, es.eobano, e.empnumeroprocesso
									) as foo
									where vrlempenho > vrlsubacao)";
	}
	if( $_REQUEST['vrlempenhomaior'] == 'N' ){
		$where[] = "p.prpnumeroprocesso not in (select 
										empnumeroprocesso 
									from(
									    select distinct
									        es.sbaid, es.eobano, e.empnumeroprocesso,
								            ((sum(es.eobvalorempenho) + sum(coalesce(ref.reevlr,0))) - sum(coalesce(can.vrlcancelado, 0))) as vrlempenho,
									        par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano) as vrlsubacao
									    from
									        par.empenho e 
									        inner join par.empenhosubacao es on es.empid = e.empid
									        left join (
									            select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as reevlr, ep.empcodigoespecie, es1.sbaid, es1.eobano
									            from par.empenho ep
									                inner join par.empenhosubacao es1 on es1.empid = ep.empid
									            where ep.empcodigoespecie in ('02')
									            group by 
									                ep.empnumeroprocesso,
									                ep.empcodigoespecie,
									                ep.empidpai, es1.sbaid, es1.eobano
									        ) as ref on ref.empidpai = e.empid and ref.sbaid = es.sbaid and ref.eobano = es.eobano
									        left join (
									            select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlcancelado, ep.empcodigoespecie, es1.sbaid, es1.eobano
									            from par.empenho ep
									                inner join par.empenhosubacao es1 on es1.empid = ep.empid
									            where ep.empcodigoespecie in ('03', '04', '13')
									            group by 
									                ep.empnumeroprocesso,
									                ep.empcodigoespecie,
									                ep.empidpai, es1.sbaid, es1.eobano
									        ) as can on can.empidpai = e.empid and can.sbaid = es.sbaid and can.eobano = es.eobano
									    where 
									        e.empcodigoespecie not in ('03', '04', '13', '02')
									    group by 
									        es.sbaid, es.eobano, e.empnumeroprocesso
									) as foo
									where vrlempenho > vrlsubacao)";
	}
	
	
	if($_REQUEST['reformulacao']){
		if($_REQUEST['reformulacao'] == 't'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid WHERE dopreformulacao = true)";
		} elseif($_REQUEST['reformulacao'] == 'f'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid WHERE dopreformulacao = false)";
		}
	}	
	
	if($_REQUEST['requisicao'] == 'exportarexcel'){		
		$cabecalho = array("Nº Processo", "Nº Documenta", "Município","UF", "Valor empenhado(R$)","QTD subações","Tipo", 'Entidade Habilitada');
		$colunaAcoes = "substring(p.prpnumeroprocesso from 1 for 5)||'.'|| substring(p.prpnumeroprocesso from 6 for 6)||'/'||
			   						substring(p.prpnumeroprocesso from 12 for 4)||'-'|| substring(p.prpnumeroprocesso from 16 for 2) as numeroprocesso,";
		$colunaSub = "";
		$colunaHabilita = 'iue.iuesituacaohabilita as habilitado';
	} else {	
		$cabecalho = array("&nbsp;","Nº Processo", "Nº Documenta", "Município","UF", "Valor empenhado(R$)","QTD subações","Tipo", 'Entidade Habilitada');

		$consultaDocumenta  = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
		$insereDocumenta  = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.prpnumeroprocesso||'\', \'empenhoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";
	
		$colunaAcoes = "'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenho(\''||p.prpnumeroprocesso||'\', this, \'lista\');\">'||
							case when p.prpdocumenta is not null then $consultaDocumenta
								else $insereDocumenta
							end || '&nbsp;<img src=../imagens/money.gif style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/solicitacaoEmpenhoPar&acao=A&processo=' || p.prpnumeroprocesso || '&prpid='||p.prpid||'&inuid='||p.inuid||'\',\'Empenho\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\"> ' || 
					   	' </center>' as acoes,
						(
                                    CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.prpnumeroprocesso) > 0 THEN
                                        '<span class=\"processoDetalhes processo_detalhe\" >'
                                    ELSE
                                        '<span class=\"processo_detalhe\" >'
                                    END
                                )
                                || substring(p.prpnumeroprocesso from 1 for 5)||'.'|| substring(p.prpnumeroprocesso from 6 for 6)||'/'||
			   						substring(p.prpnumeroprocesso from 12 for 4)||'-'|| substring(p.prpnumeroprocesso from 16 for 2) || '</span>' as numeroprocesso,";
		
		$colunaSub = "acoes,";
		
		$colunaHabilita = "case when iue.iuesituacaohabilita = 'Habilitado' then
			              		'<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' 
			              	else 
			              		'<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\" onclick=\"verificaHabilita(\''||p.prpcnpj||'\');\" style=cursor:pointer;></center>' end as habilitado";
	}
// 	ver($joinDivergente);
	$sql = "select distinct
				$colunaSub
				numeroprocesso,
				prpdocumenta,
				descricao,
				uf,
				valorempenhado,
				qtdsubacaoempenhada,
				tipo,
				habilitado
				from(
						SELECT DISTINCT
							$colunaAcoes
                                
			   	$colunaDivergente
			   	p.prpdocumenta, 
			   	CASE WHEN iu.itrid = 1 THEN '' ELSE CASE WHEN iu.estuf = 'DF' THEN '' ELSE m.mundescricao END END as descricao, 
			   	CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE m.estuf END as uf, 
			   	(COALESCE((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso), 0.00)) as valorempenhado,
			   (SELECT COUNT(emp.sbaid) 
			   		FROM par.empenho eo 
			   		INNER JOIN par.empenhosubacao emp ON emp.empid = eo.empid and eobstatus = 'A' and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' 
			   		WHERE eo.empnumeroprocesso = p.prpnumeroprocesso) as qtdsubacaoempenhada,
			   CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END AS tipo,
			   $colunaHabilita
		FROM par.instrumentounidade iu
			INNER JOIN par.processopar p ON p.inuid = iu.inuid ".$condicaoProcessoPar."
			LEFT JOIN territorios.municipio m ON m.muncod = iu.muncod
			$joinGrupoMun
		--	LEFT JOIN territorios.estado e ON e.estuf = iu.estuf
			$inner
			LEFT JOIN par.instrumentounidadeentidade iue on iue.inuid = iu.inuid and iue.iuestatus = 'A' and iue.iuedefault = true
			$joinDivergente
		WHERE
			p.prpstatus = 'A'			
		".(($where)?" and ".implode(" AND ", $where):"")."
		ORDER BY tipo, uf, descricao
) as foo
	";
		
// 	ver(simec_htmlentities($sql));

	if($_REQUEST['requisicao'] == 'exportarexcel'){
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%',$par2);
		exit;
	} else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
			//ver( simec_htmlentities($sql), d );
			$param['ordena'] = false;
            $db->monta_lista($sql,$cabecalho,20,10,'N','center','N','formprocesso', '', '', '', $param);
//             $db->monta_lista_simples($sql,$cabecalho,30,10,'N','','N',true, '', '', true);
        }
    }

$wsusuario = WEB_SERVICE_SIOP_USUARIO;
$wssenha = WEB_SERVICE_SIOP_SENHA;

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";
?>
