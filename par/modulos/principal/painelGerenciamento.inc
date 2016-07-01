<?php

if($_REQUEST['painel_estrategico']) {
	
	if($_REQUEST['painel_estrategico']==2) {

		$agrupador2 = array(
						'tipoobra' => array(
													'codigo'    => 'tipoobra',
													'descricao' => '09. Pro-Infância/Quadra/Cobertura'
						),
						'regdescricao' => array(
													'codigo'    => 'regdescricao',
													'descricao' => '08. Região'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '04. Estado'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '05. Municipio'
						),
						'obra' => array(
													'codigo'    => 'obra',
													'descricao' => '06. Obra'
						)
						
						);
		
	} else {
		$agrupador2 = array(
						'regdescricao' => array(
													'codigo'    => 'regdescricao',
													'descricao' => '08. Região'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '04. Estado'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '05. Municipio'
						),
						'obra' => array(
													'codigo'    => 'obra',
													'descricao' => '06. Obra'
						)
						
						);		
	}
	

					
	$_POST['esferafiltro'] = "t";
	
	if($_REQUEST['ano']) $_POST['preanoselecao'] = $_REQUEST['ano'];
	
	switch($_REQUEST['situacao']) {
		case '5':
			$stSql_esd = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37 AND 
								esdid in (228,360,365,366,367,683,754,755)
							ORDER BY
								descricao ";			
			break;
		case '4':
			$stSql_esd = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37 AND 
								esdid in (214)
							ORDER BY
								descricao ";			
			break;
		case '3':
			$stSql_esd = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37 AND 
								esdid in (194,215,218,195,217,213,221,212,210,211)
							ORDER BY
								descricao ";			
			break;
		case '2':
			$stSql_esd = " SELECT DISTINCT
								esdid as codigo,
								esddsc as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37 AND 
								esdid not in (229,193)
							ORDER BY
								descricao ";			
			break;
	}
	
	if($_REQUEST['painel_estrategico']==2) {
		$tpSql_pto = "SELECT			t.ptoid as codigo, 
									ptodescricao  as descricao
					FROM 
						obras.pretipoobra t
					WHERE t.ptoclassificacaoobra IN ('C', 'Q')
					ORDER BY 1";

	} else {
		if($_REQUEST['painel_estrategico']==3){
			$tpSql_pto = "SELECT			t.ptoid as codigo, 
										ptodescricao  as descricao
						FROM 
							obras.pretipoobra t
						WHERE t.ptoclassificacaoobra = 'P'
						AND t.ptoid in (42,43,44,45)
						ORDER BY 1";
		}else{
			$tpSql_pto = "SELECT			t.ptoid as codigo, 
										ptodescricao  as descricao
						FROM 
							obras.pretipoobra t
						WHERE t.ptoclassificacaoobra = 'P'
						ORDER BY 1";
		}
	}
	
	
	$script_est = "jQuery(document).ready(function() {obras_exibeRelatorioGeral('exibir');escondeTabela()});";
	
	
}

if ($_POST['tipo'] == 2){
	ini_set("memory_limit","2048M");
	$true = true;
	include("painelGerenciamento_resultado.inc");
	exit;
}

include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

$abasPar = array( 0 => array( "descricao" => "Painel Gerencial",
							  "link"	  => "par.php?modulo=principal/painelGerenciamento&acao=A" ),
				  1 => array( "descricao" => "Mapa Gerencial",
							  "link"	  => "par.php?modulo=principal/mapaGerenciamento&acao=A" )  
						 );

echo montarAbasArray( $abasPar, "par.php?modulo=principal/painelGerenciamento&acao=A" );

monta_titulo( $titulo_modulo, '' );

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	<?=$script_est ?>
	
	function escondeTabela()
	{
		jQuery("#tabela_filtros").hide();
		jQuery("#btn_esconder").hide();
		jQuery("#btn_exibir").show();
		jQuery("#tr_botoes").show();
	}
	
	function exibirFiltros()
	{
		jQuery("#btn_esconder").show();
		jQuery("#btn_exibir").hide();
		jQuery("#tabela_filtros").show();
	}
	
	function esconderFiltros()
	{
		jQuery("#btn_esconder").hide();
		jQuery("#btn_exibir").show();
		jQuery("#tabela_filtros").hide();
	}
	
	function obras_exibeRelatorioGeral(tipo){

		var formulario = document.filtro;
		var agrupador  = $( 'colunas'  );
		var agrupador2 = $( 'colunas2' );

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();
		selectAllOptions( formulario.colunas  );
		selectAllOptions( formulario.colunas2 );
		
		if( tipo == 'exibir' ){

			$('visualizar').disabled = true;
			$('xls').disabled = true;
			var cbregcod = $('regcod');
			var regcod = '';
			
			if( regcod.value == '' ){
				regcod = '';
			}else{
				regcod = cbregcod.value;
			}
		 	
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos um agrupador!' );
				$('visualizar').disabled = false;
				return false;
			}

			if ( !agrupador2.options.length ){
				alert( 'Favor selecionar ao menos uma coluna!' );
				$('visualizar').disabled = false;
				return false;
			}
			
			// Seleção 
			var selObj = document.getElementsByName('selecao');
			var selecao;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					selecao = selObj[i].value;
			  	}
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

			// Colunas 
			selectAllOptions( agrupador2 );
			var colunas2 = "";
			var selObj = $('colunas2');
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if (selObj.options[i].selected) {
					colunas2 +="{"+selObj.options[i].value+"}";
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
			
			// Situação FNDE 
			var selObj = $('esdid_');
			selectAllOptions( selObj );
			var esdid_ = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					esdid_ +="{"+selObj.options[i].value+"}";
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
			
			// UF 
			var selObj = $('estuf');
			selectAllOptions( selObj );
			var estuf = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					estuf +="{'"+selObj.options[i].value+"'}";
			  	}
			}
			
			// Municipio 
			var selObj = $('muncod');
			selectAllOptions( selObj );
			var muncod = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					muncod +="{'"+selObj.options[i].value+"'}";
			  	}
			}
			
			// Analista 
			var selObj = $('usucpfanalista');
			selectAllOptions( selObj );
			var usucpfanalista = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					usucpfanalista +="{'"+selObj.options[i].value+"'}";
			  	}
			}

			// Resolução 
			var selObj = $('resid');
			selectAllOptions( selObj );
			var resid = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					resid +="{'"+selObj.options[i].value+"'}";
			  	}
			}

			// Pagamento 
			var selObj = document.getElementsByName('pagamento');
			var pagamento;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					pagamento = selObj[i].value;
			  	}
			}

			// Termo 
			var selObj = document.getElementsByName('assinado');
			var assinado;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					assinado = selObj[i].value;
			  	}
			}
			
			// Carga 
			var selObj = document.getElementsByName('carga');
			var carga;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					carga = selObj[i].value;
			  	}
			}
			
			// MCMV 
			var selObj = document.getElementsByName('mcmv');
			var mcmv;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					mcmv = selObj[i].value;
			  	}
			}

			var nottpmid  		  = $('tpmid_campo_excludente').checked;
			var notesdid  		  = $('esdid_campo_excludente').checked;
			var notesdid_  		  = $('esdid__campo_excludente').checked;
			var notptoid  		  = $('ptoid_campo_excludente').checked;
			var notestuf  		  = $('estuf_campo_excludente').checked;
			var notmuncod 		  = $('muncod_campo_excludente').checked;
			var notusucpfanalista = $('usucpfanalista_campo_excludente').checked;			
			var notresid 		  = $('resid_campo_excludente').checked;			
//			var empenho 		  = $('empenho').checked;			
			var esferafiltro 	  = jQuery("[name='esferafiltro']").val();
			
			var div = $('div_resposta');

			$('loader-container').show();
			
			//Ano Seleção
			if( jQuery("[name='preanoselecao']").val() ){
				var anoselecao = jQuery("[name='preanoselecao']").val();
				if( jQuery("[name='preanometa']").val() ){
					var preanometa = jQuery("[name='preanometa']").val();
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&anoselecao='+anoselecao+'&preanometa='+preanometa;
				} else {
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&anoselecao='+anoselecao;
				}
			} else {
				if( jQuery("[name='preanometa']").val() ){
					var preanometa = jQuery("[name='preanometa']").val();
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&preanometa='+preanometa;
				} else {
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro;
				}
			}
			
			var myAjax = new Ajax.Request(
					'/par/par.php?modulo=principal/painelGerenciamento_resultado&acao=A',
		    		{
		    			method: 'post',
		    			parameters: param,
		    			asynchronous: false,
		    			onComplete: function(resp) {
							$('tr_resposta').style.display = 'table-row';
		    				extrairScript(resp.responseText);
		    				div.innerHTML = resp.responseText;
		    				$('visualizar').disabled = false;
		    				$('xls').disabled = false;
		    				$('loader-container').hide();
		    			}
		    		});
			
			
		} else {
			
			$('visualizar').disabled = true;
			$('xls').disabled = true;
			var cbregcod = $('regcod');
			var regcod = '';
			
			if( regcod.value == '' ){
				regcod = '';
			}else{
				regcod = cbregcod.value;
			}
		 	
			if ( !agrupador.options.length ){
				alert( 'Favor selecionar ao menos uma agrupador!' );
				$('visualizar').disabled = false;
				return false;
			}

			if ( !agrupador2.options.length ){
				alert( 'Favor selecionar ao menos uma coluna!' );
				$('visualizar').disabled = false;
				return false;
			}
			
			// Seleção 
			var selObj = document.getElementsByName('selecao');
			var selecao;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					selecao = selObj[i].value;
			  	}
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
			
			// Colunas 
			selectAllOptions( agrupador2 );
			var colunas2 = "";
			var selObj = $('colunas2');
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if (selObj.options[i].selected) {
					colunas2 +="{"+selObj.options[i].value+"}";
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
			
			// Situação FNDE 
			var selObj = $('esdid_');
			selectAllOptions( selObj );
			var esdid_ = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					esdid_ +="{"+selObj.options[i].value+"}";
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
			
			// UF 
			var selObj = $('estuf');
			selectAllOptions( selObj );
			var estuf = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					estuf +="{'"+selObj.options[i].value+"'}";
			  	}
			}
			
			// Municipio 
			var selObj = $('muncod');
			selectAllOptions( selObj );
			var muncod = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					muncod +="{'"+selObj.options[i].value+"'}";
			  	}
			}
			
			// Analista 
			var selObj = $('usucpfanalista');
			selectAllOptions( selObj );
			var usucpfanalista = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					usucpfanalista +="{'"+selObj.options[i].value+"'}";
			  	}
			}

			// Resolução 
			var selObj = $('resid');
			selectAllOptions( selObj );
			var resid = "";
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if ( selObj.options[i].value != "" ) {
					resid +="{'"+selObj.options[i].value+"'}";
			  	}
			}

			// Pagamento 
			var selObj = document.getElementsByName('pagamento');
			var pagamento;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					pagamento = selObj[i].value;
			  	}
			}

			// Termo 
			var selObj = document.getElementsByName('assinado');
			var assinado;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					assinado = selObj[i].value;
			  	}
			}
			
			// Carga 
			var selObj = document.getElementsByName('carga');
			var carga;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					carga = selObj[i].value;
			  	}
			}
			
			// MCMV 
			var selObj = document.getElementsByName('mcmv');
			var mcmv;
			var i;
			for (i=0; i<selObj.length; i++) {
				if ( selObj[i].checked ) {
					mcmv = selObj[i].value;
			  	}
			}

			var nottpmid  		  = $('tpmid_campo_excludente').checked;
			var notesdid  		  = $('esdid_campo_excludente').checked;
			var notesdid_  		  = $('esdid__campo_excludente').checked;
			var notptoid  		  = $('ptoid_campo_excludente').checked;
			var notestuf  		  = $('estuf_campo_excludente').checked;
			var notmuncod 		  = $('muncod_campo_excludente').checked;
			var notusucpfanalista = $('usucpfanalista_campo_excludente').checked;			
			var notresid 		  = $('resid_campo_excludente').checked;			
//			var empenho 		  = $('empenho').checked;			
			var esferafiltro 	  = jQuery("[name='esferafiltro']").val();
			
			var div = $('div_resposta');

			$('loader-container').show();
			
			//Ano Seleção
			if( jQuery("[name='preanoselecao']").val() ){
				var anoselecao = jQuery("[name='preanoselecao']").val();
				if( jQuery("[name='preanometa']").val() ){
					var preanometa = jQuery("[name='preanometa']").val();
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&anoselecao='+anoselecao+'&preanometa='+preanometa;
				} else {
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&anoselecao='+anoselecao;
				}
			} else {
				if( jQuery("[name='preanometa']").val() ){
					var preanometa = jQuery("[name='preanometa']").val();
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro+'&preanometa='+preanometa;
				} else {
					param = '&selecao='+selecao+'&assinado='+assinado+'&mcmv='+mcmv+'&carga='+carga+'&pagamento='+pagamento+'&nottpmid='+nottpmid+'&notesdid_='+notesdid_+'&notesdid='+notesdid+'&notptoid='+notptoid+'&notestuf='+notestuf+'&notmuncod='+notmuncod+'&notusucpfanalista='+notusucpfanalista+'&notresid='+notresid+'&regcod='+regcod+'&tpmid='+tpmid+'&colunas='+colunas+'&colunas2='+colunas2+'&esdid_='+esdid_+'&esdid='+esdid+'&ptoid='+ptoid+'&estuf='+estuf+'&muncod='+muncod+'&usucpfanalista='+usucpfanalista+'&datatramite_fim='+datatramite_fim.value+'&datatramite_inicio='+datatramite_inicio.value+'&resid='+resid+'&esferafiltro='+esferafiltro;
				}
			}
			
			document.getElementById('tipo').value = 2;
			
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';	
		//	ajaxRelatorio();
			formulario.submit();
			$('visualizar').disabled = false;
		    $('xls').disabled = false;
			janela.focus();

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
	
	function abreObrasPainel(preid,muncod)
	{
		var url = "../par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid=" + preid + "&muncod=" + muncod;
		window.open(url,'PAR','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}

</script>
<form action="" method="post" name="filtro" id="filtro"> 
	<div id="loader-container" style="position: absolute; margin-left: 600px">
		<div id="loader">
			<img src="../imagens/wait.gif" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</div>
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="tipo" id="tipo" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	
	<table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" width="10%">Seleção:</td>
			<td>
				<?php 
					if($_REQUEST['selecao']){
						$selecao = $_REQUEST['selecao'];
					} else {
						$selecao = 'pac';
					}
				?>
				<input <?php echo $selecao == "pac" ? "checked='checked'" : "" ?> type="radio" name="selecao" value="pac" >PAC
				<input <?php echo $selecao == "par" ? "checked='checked'" : "" ?> type="radio" name="selecao" value="par" >PAR
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Região:</td>
			<td>
				<?php 
				// Regiões
				$stSql = " SELECT 
								regcod as codigo, 
								regdescricao as descricao
							FROM 
								territorios.regiao
							ORDER BY
								descricao ";
				
				$db->monta_combo( 'regcod', $stSql, 'S', 'Todos', '', '','','243','N','regcod' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="10%">Colunas</td>
			<td>
				<?php
					// Início dos agrupadores
					$agrupador3 = new Agrupador('filtro','');
					// Dados padrão de destino (nulo)
					$destino2 = array(
									'qtd' => array(
													'codigo'    => 'qtd',
													'descricao' => '01. Quantidade de Obras'
									),
									'vlr' => array(
													'codigo'    => 'vlr',
													'descricao' => '02. Valor das Obras'
									),
									'vlrempenho' => array(
													'codigo'    => 'vlrempenho',
													'descricao' => '03. Valores Empenhados das Obras'
									)
								);
					
					// Dados padrão de origem
					$origem2 = array(
						'esddsc' => array(
													'codigo'    => 'esddsc',
													'descricao' => '04. Situação da Obra'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '05. Estado'
						),
						'muncod' => array(
													'codigo'    => 'muncod',
													'descricao' => '06. Código do Municipio'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '07. Municipio'
						),
						'nomedaobra' => array(
													'codigo'    => 'nomedaobra',
													'descricao' => '08. Nome da Obra'
						),
						'resolucao' => array(
													'codigo'    => 'resolucao',
													'descricao' => '09. Resolução'
						),
						'entcodent' => array(
													'codigo'    => 'entcodent',
													'descricao' => '10. Código da Escola'
						),
						'entnome' => array(
													'codigo'    => 'entnome',
													'descricao' => '11. Nome da Escola'
						),
						'preesfera' => array(
													'codigo'    => 'preesfera',
													'descricao' => '12. Esfera'
						),
						'preanometa' => array(
													'codigo'    => 'preanometa',
													'descricao' => '13. Ano da Meta'
						),
						'preanoselecao' => array(
													'codigo'    => 'preanoselecao',
													'descricao' => '14. Ano da Seleção'
						)						
						
					);
					
					// exibe agrupador 
					$agrupador3->setOrigem( 'naoColunas2', null, $origem2 );
					$agrupador3->setDestino( 'colunas2', null, $destino2 );
					$agrupador3->exibir();
				?>
			</td>
		</tr>
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
						'esddsc' => array(
													'codigo'    => 'esddsc',
													'descricao' => '01. Situação da Obra'
						),
						'esddsc_' => array(
													'codigo'    => 'esddsc_',
													'descricao' => '02. Situação FNDE'
						),
						'ptodescricao' => array(
													'codigo'    => 'ptodescricao',
													'descricao' => '03. Tipo da Obra'
						),
						'uf' => array(
													'codigo'    => 'uf',
													'descricao' => '04. Estado'
						),
						'municipio' => array(
													'codigo'    => 'municipio',
													'descricao' => '05. Municipio'
						),
						'obra' => array(
													'codigo'    => 'obra',
													'descricao' => '06. Obra'
						),
						'tpmdsc' => array(
													'codigo'    => 'tpmdsc',
													'descricao' => '07. Grupo de Municípios'
						),
						'regdescricao' => array(
													'codigo'    => 'regdescricao',
													'descricao' => '08. Região'
						),
						'tipoobra' => array(
													'codigo'    => 'tipoobra',
													'descricao' => '09. Pro-Infância/Quadra/Cobertura'
						),
						'analista' => array(
													'codigo'    => 'analista',
													'descricao' => '11. Analista'
						),
						'data' => array(
													'codigo'    => 'data',
													'descricao' => '12. Data'
						),
						'resolucao' => array(
													'codigo'    => 'resolucao',
													'descricao' => '13. Resolução'
						),
						'empenhado' => array(
													'codigo'    => 'empenhado',
													'descricao' => '14. Empenho'
						),
						'esfera' => array(
													'codigo'    => 'esfera',
													'descricao' => '15. Esfera'
						),
						'muncod' => array(
													'codigo'    => 'muncod',
													'descricao' => '16. Código IBGE'
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
				
				// Grupo
				$stSql = "  SELECT 
								tpmid as codigo, 
								tpmdsc as descricao
							FROM 
								territorios.tipomunicipio
							WHERE
								tpmstatus = 'A' 
								AND tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)
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
				mostrarComboPopup( 'Situação', 'esdid',  $stSql, $stSql_esd, 'Selecione a(s) Situação(ões)' );
		?>
		<tr id="filtro_periodo" style="display:none;">
			<td class="subtitulodireita">
				Período de Tramitação
			</td>
			<td>
				<?=campo_data2('datatramite_inicio', 'N', 'S', 'Período de Tramitação', '##/##/####')?>
				&nbsp;&nbsp;até&nbsp;&nbsp;
				<?=campo_data2('datatramite_fim', 'N', 'S', 'Período de Tramitação', '##/##/####')?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				Ano de Seleção
			</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
								preanoselecao as codigo,
								preanoselecao as descricao
							FROM 
								obras.preobra
							WHERE
								prestatus = 'A' 
								AND preanoselecao > 0 ";

					$preanoselecao = $_POST['preanoselecao'];
					$db->monta_combo( "preanoselecao", $sql, 'S', 'Selecione...', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				Esfera
			</td>
			<td>
				<?php
					$arr = array(
									array( 'codigo' => 't', 'descricao' => 'Todas' ),
									array( 'codigo' => 'm', 'descricao' => 'Municipal' ),
									array( 'codigo' => 'e', 'descricao' => 'Estadual' )
					);

					$esferafiltro = $_POST['esferafiltro'];
					$db->monta_combo( "esferafiltro", $arr, 'S', '', '', '' );
				?>
			</td>
		</tr>
		<?php 
				
				// Situação
				$stSql = " SELECT DISTINCT
								CASE WHEN esdid IN(194,215,218,212,217,211,210) THEN '194,215,218,212,217,211,210' ELSE esdid::varchar END as codigo,
								CASE WHEN esdid IN(194,215,218,212,217,211,210) THEN 'Em análise' ELSE esddsc END as descricao
							FROM 
								workflow.estadodocumento
							WHERE
								esdstatus = 'A' AND
								tpdid = 37
							ORDER BY
								descricao ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação FNDE', 'esdid_',  $stSql, '', 'Selecione a(s) Situação(ões)' );
				
				
				// Tipo
				$tpSql = " SELECT 
								t.ptoid as codigo, 
								ptodescricao  as descricao
							FROM 
								obras.pretipoobra t
							INNER JOIN obras.preobra o ON o.ptoid = t.ptoid 
							ORDER BY
								1";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Tipo', 'ptoid',  $tpSql, $tpSql_pto, 'Selecione o(s) Tipo(s)' );
				
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
				mostrarComboPopup( 'Estado', 'estuf',  $ufSql, '', 'Selecione o(s) Estado(s)' );
				
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
				
				// Analistas
				$analistaSql = "SELECT 
									usu.usucpf AS codigo,
									usu.usunome AS descricao	
								FROM seguranca.usuario usu
								INNER JOIN seguranca.perfilusuario pusu ON usu.usucpf = pusu.usucpf
								WHERE pusu.pflcod = 468
								ORDER BY usu.usunome";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Analistas', 'usucpfanalista',  $analistaSql, '', 'Selecione o(s) Analista(s)' );
				
				// Resoluções
				$analistaSql = "SELECT
									resid as codigo,
									resnumero as descricao
								FROM
									par.resolucao
								WHERE
									resstatus = 'A'";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Resoluções', 'resid',  $analistaSql, '', 'Selecione a(s) Resolução(ões)' );

			?>
		<tr>
			<td class="subtitulodireita">Pagamento</td>
			<td>
				<input <?php echo $_REQUEST['pagamento'] == "0" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="0" >Ver todos
				<input <?php echo $_REQUEST['pagamento'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="1" >Pagamento Solicitado
				<input <?php echo $_REQUEST['pagamento'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="2" >Pagamento Não Solicitado
				<input <?php echo $_REQUEST['pagamento'] == "3" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="3" >Pagamento efetivado
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Termo</td>
			<td>
				<input <?php echo $_REQUEST['assinado'] == "3" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" >Ver Todos
				<input <?php echo $_REQUEST['assinado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="1" >Assinado
				<input <?php echo $_REQUEST['assinado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" >Não Assinado
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">MCMV</td>
			<td>
				<input <?php echo $_REQUEST['mcmv'] == "todos" ? "checked='checked'" : "" ?> type="radio" name="mcmv" value="todos" >Ver Todos
				<input <?php echo $_REQUEST['mcmv'] == "t" ? "checked='checked'" : "" ?> type="radio" name="mcmv" value="t" >Sim
				<input <?php echo $_REQUEST['mcmv'] == "f" ? "checked='checked'" : "" ?> type="radio" name="mcmv" value="f" >Não
			</td>
		</tr>
		<?php
		if( $db->testa_superuser() == true ){
		?>
			<tr>
				<td class="subtitulodireita">Carga</td>
				<td>
					<input <?php echo $_REQUEST['carga'] == "todos" ? "checked='checked'" : "" ?> type="radio" name="carga" value="todos" >Ver Todos
					<input <?php echo $_REQUEST['carga'] == "t" ? "checked='checked'" : "" ?> type="radio" name="carga" value="t" >Sim
					<input <?php echo $_REQUEST['carga'] == "f" ? "checked='checked'" : "" ?> type="radio" name="carga" value="f" >Não
				</td>
			</tr>
		<?php } ?>
		<tr>
			<td class="subtitulodireita">Ano da Meta</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT preanometa as codigo, preanometa as descricao FROM obras.preobra WHERE preanometa IS NOT NULL";

					$preanometa = $_POST['preanometa'];
					$db->monta_combo( "preanometa", $sql, 'S', 'Selecione...', '', '' );
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC" width="10%"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" id="visualizar" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
				<input type="button" id="xls" value="Gerar Excel" onclick="obras_exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
		<tr id="tr_botoes" style="display:none"  >
			<td colspan="2" style="background-color: white;">
				<input type="button" id="btn_exibir" name="btn_exibir" value="Exibir filtros" onclick="exibirFiltros()" />
				<input type="button" id="btn_esconder" name="btn_esconder" value="Esconder filtros" onclick="esconderFiltros()" />
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
$('loader-container').hide();

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