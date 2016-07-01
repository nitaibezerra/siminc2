<?php

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	function obras_exibeRelatorioGeral(tipo){
		
		var formulario = document.filtro;
		var agrupador = $( 'colunas' );
		var visualizar = $( 'visualizar' );
		visualizar.disabled = true;
		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		selectAllOptions( formulario.colunas );
		
		if( tipo == 'exibir' ){

			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				return false;
			}
			
			// Agrupador 
			selectAllOptions( agrupador );
			var colunas = "";
			var selObj = $('colunas');
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if (selObj.options[i].selected) {
					colunas +="{"+selObj.options[i].value+"}";
			  	}
			}
			
			// UF 
			var selObj = $('estuf');
			selectAllOptions( selObj );
			var estuf = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					estuf +="{\'"+selObj.options[i].value+"\'}";
			  	}
			}
			
			// Grupo de Municipio 
			var selObj = $('tpmid');
			selectAllOptions( selObj );
			var tpmid = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					tpmid +="{"+selObj.options[i].value+"}";
			  	}
			}
			
			// Situação 
			var selObj = $('esdid');
			selectAllOptions( selObj );
			var esdid = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					esdid +="{"+selObj.options[i].value+"}";
			  	}
			}
			
			// Municipio 
			var selObj = $('muncod');
			selectAllOptions( selObj );
			var muncod = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					muncod +="{\'"+selObj.options[i].value+"\'}";
			  	}
			}
			
			// Tipo 
			var selObj = $('ptoid');
			selectAllOptions( selObj );
			var ptoid = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					ptoid +="{"+selObj.options[i].value+"}";
			  	}
			}
			
			var notestuf  			= $('estuf_campo_excludente').checked;
			var nottpmid  			= $('tpmid_campo_excludente').checked;
			var notesdid  			= $('esdid_campo_excludente').checked;
			var notmuncod 			= $('muncod_campo_excludente').checked;			
			var notptoid 			= $('ptoid_campo_excludente').checked;	
					
			
			var div = $('div_resposta');
			
			var myAjax = new Ajax.Request(
					'/par/par.php?modulo=relatorio/relatorioArquivos_resultado&acao=A',
		    		{
		    			method: 'post',
		    			parameters: '&notestuf='+notestuf+'&nottpmid='+nottpmid+'&notesdid='+notesdid+'&notmuncod='+notmuncod+'&notptoid='+notptoid+'&colunas='+colunas+'&estuf='+estuf+'&tpmid='+tpmid+'&esdid='+esdid+'&muncod='+muncod+'&ptoid='+ptoid,
		    			asynchronous: false,
		    			onComplete: function(resp) {
							$('tr_resposta').style.display = 'table-row';
		    				extrairScript(resp.responseText);
		    				div.innerHTML = resp.responseText;
		    				visualizar.disabled = false;
		    			}
		    		});
			
			
		}
		
	}

	function onOffCampo( campo ) {
		var div_on  = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input   = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}

	function Mascara_Hora(Hora){ 
		var hora01 = ''; 
		hora01 = hora01 + Hora; 
		if (hora01.length == 2){ 
			hora01 = hora01 + ':'; 
			document.forms[0].Hora.value = hora01; 
		} 
		if (hora01.length == 5){ 
			Verifica_Hora(); 
		} 
	} 
		           
	function Verifica_Hora( hora ){ 
		if( hora.value.length > 3 ){
			hrs = (hora.value.substring(0,2)); 
			min = (hora.value.substring(3,5)); 
		}else{
			hrs = 0; 
			min = (hora.value.substring(hora.value.length-2,hora.value.length)); 
		}
		               
		estado = ""; 
		if ((hrs < 00 ) || (hrs > 23) || ( min < 00) ||( min > 59)){ 
			estado = "errada"; 
		} 
		               
		if (hora.value == "") { 
			estado = ""; 
		} 
	
		if (estado == "errada") { 
			hora.value = "";
			alert("Hora inválida!"); 
			hora.focus(); 
		} 
	} 
			

</script>
<form action="" method="post" name="filtro" id="filtro"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Agrupadores</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador = new Agrupador('filtro','');
					
					// Dados padrão de destino (nulo)
					$destino = isset( $agrupador2 ) ? $agrupador2 : array();
					
					// Dados padrão de origem
					$origem = array(
						'orgao' => array(
													'codigo'    => 'orgao',
													'descricao' => '01. Orgão'
						),
						'nomeobra' => array(
													'codigo'    => 'nomeobra',
													'descricao' => '02. Nome da Obra'
						),
						'situacao' => array(
													'codigo'    => 'situacao',
													'descricao' => '03. Situação da Obra'
						),
						'MunicipioObra' => array(
													'codigo'    => 'municipioobra',
													'descricao' => '04. Município da Obra'
						),
						'grupo' => array(
													'codigo'    => 'grupo',
													'descricao' => '05. Grupo do Município da Obra'
						),
						'tipoobra' => array(
													'codigo'    => 'tipoobra',
													'descricao' => '06. Tipo da Obra'
						),
						'extensao' => array(
													'codigo'    => 'extensao',
													'descricao' => '07. Extensão do Arquivo'
						),
						'ufusuario' => array(
													'codigo'    => 'ufusuario',
													'descricao' => '08. UF'
						),
						'nomearquivo' => array(
													'codigo'    => 'nomearquivo',
													'descricao' => '09. Dados do Arquivo'
						)
						
					);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoColunas', null, $origem );
					$agrupador->setDestino( 'colunas', null, $destino );
					$agrupador->exibir();
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloesquerda" colspan="2">
				<strong>Filtros</strong>
			</td>
		</tr>
			<?php
				// UF 
				$ufSql = " SELECT	
								po.estuf AS codigo,
								estdescricao AS descricao
							FROM 
								territorios.estado est
							INNER JOIN
								obras.preobra po ON po.estuf = est.estuf
							WHERE
							 	po.prestatus = 'A'
							ORDER BY
								estdescricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'UF', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
				// Municípios
				$munSql = " SELECT
								tm.muncod AS codigo,
								tm.estuf || ' - ' || tm.mundescricao AS descricao
							FROM 
								territorios.municipio tm
							INNER JOIN
								obras.preobra po ON po.muncod = tm.muncod
							WHERE
							 	po.prestatus = 'A'
							ORDER BY
								mundescricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Município', 'muncod',  $munSql, '', 'Selecione o(s) Município(s)' );
				// Grupo de Municipios
				$stSql = "  SELECT 
								tpmid as codigo, 
								tpmdsc as descricao
							FROM 
								territorios.tipomunicipio
							WHERE
								tpmstatus = 'A' 
								AND gtmid = 7
							ORDER BY
								descricao  ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Grupo de Municípios', 'tpmid',  $stSql, '', 'Selecione o(s) Grupo(s)' );
				// Situação
				$stSql = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37
							ORDER BY
								descricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação', 'esdid',  $stSql, '', 'Selecione a(s) Situação(ões)' );
				// Tipo
				$tpSql = " SELECT 
								ptoid as codigo, 
								ptodescricao  as descricao
							FROM 
								obras.pretipoobra
							WHERE
								ptostatus = 'A'
							ORDER BY
								ptoid";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo', 'ptoid',  $tpSql, '', 'Selecione o(s) Tipo(s)' );
		?>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="visualizar" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
			</td>
		</tr>
		<tr id="tr_resposta" >
			
			<td colspan="2" style="background-color: white;">
				<div id="div_resposta">
				</div>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
<!--

jQuery(document).ready(function(){

	jQuery('#esdid').click(function(){
		
		var div_on    = document.getElementById( 'esdid_campo_on' );
		
		if(div_on.style.display == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();	
		}

	});

	jQuery('#tr_esdid').click(function(){
		
		if(jQuery('#esdid_campo_on').css('display') == 'block'){
			
			jQuery('#filtro_periodo').show();
		}else{
			
			jQuery('#filtro_periodo').hide();
		}
	});
	
});

//-->
</script>