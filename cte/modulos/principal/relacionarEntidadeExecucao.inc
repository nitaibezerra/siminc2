<?php
//
// $Id$
//

//include_once( APPRAIZ . "includes/classes/modelo/entidade/Entidade.class.inc" );

cte_verificaSessao();

if( $_REQUEST["req"] == "gravarSessao" ){
	
	$_SESSION["arEntidadesSelecionadas"] = $_REQUEST["arChecks"];

}

//$obEntidade= new Entidade(); 
$arEscolas = montarRelacionamentoEscolasExecucaoPorMuncod( $_REQUEST["muncod"] );
$arEntid = recuperarEscolasPorMuncod( $_REQUEST["muncod"] );

$arEntidadesSelecionadas = $_SESSION["arEntidadesSelecionadas"];

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
	
    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
        #toolTipQtd{
        	width: 100px; 
        	height: 50px; 
        	background: #fff; 
        	position: absolute;
        	border: 1px #555 solid;
        	padding: 5px;
        	display: none;
        }
        
    </style>
  </head>
  <body >
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmEscolas" onsubmit="return validarFrmEscolas();">
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Relacionar Escolas</div>
          <div style="margin: 0; padding: 0; height: 450px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
			<?php $db->monta_lista_simples($arEscolas, array( '', 'Escolas', 'INEP' ), 5000, 10, 'N', '100%'); ?>
          </div>
          <br />
        </td>
      </tr>
      <tr>
      	<td>
      		<input type="button" value="Ok" onclick="window.close();">
      	</td>
     </tr>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--
    /**
     * 
     */
    function selecionarTodas()
    {
        var items = Form.getElements($('frmEscolas'));

        for (var i = 0; i < items.length; i++) {
            if (items[i].getAttribute('type') == 'checkbox' &&
                /entid/.test(items[i].getAttribute('id')))
            {
                items[i].checked = $('selecionar').checked;
            }
        }
    }

    function validarFrmEscolas()
    {
        var checks = $('frmEscolas').getInputs('checkbox', 'entid[]');
        var submit = true;
        
        for (var i = 0, length = checks.length; i < length; i++) {
            if (checks[i].checked || checks[i].checked == 'checked') {
                var qfaqtd = $('frmEscolas').getInputs('text');
				
				if(qfaqtd[i+1].value == '' || qfaqtd[i+1].value == 0 ){
					 //qfaqtd.setStyle({backgroundColor: '#ffd'});
					 submit = false;
				}
				
            }
        }

		 if (submit == false){
		 	alert('Para escolas selecionadas, a quantidade deve ser informada!');
		  	return submit;
		 }else{
		  	return submit;
		 }
		 

    }
    
    function preencherTodasQtds(){
    	var qtdPadrao = document.getElementById( 'qfaqtdTotal' ).value;
        var arInputQtd = document.getElementsByTagName('input');
        	
        if( qtdPadrao ){
	    	for( i = 0; i < arInputQtd.length; i++ ){
	    		if( arInputQtd[i].name.substr( 0, 6 ) == 'qfaqtd' )
    				arInputQtd[i].value = qtdPadrao;
	    	}
    	}
    }
    
    function mouseOverToolTip( objeto ){
    	MouseOver( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "block";    
    }
    
    function mouseOutToolTip( objeto ){
    	MouseOut( objeto );
		document.getElementById( 'toolTipQtd' ).style.display = "none";    
    }
	
	itens_selecionados = 0;
	function adiciona_item( cod, descricao, checkbox ){
	
		maximo = window.opener.document.getElementById('exeqtd').value;				
		maximoCampo = window.opener.document.getElementById('exeqtd');
		
		var tabela = window.opener.document.getElementById( "tabelaEscolas" );
		var tbody = window.opener.document.getElementById( "tbodyTabela" );
		var qtdLinha =  tabela.rows.length - 1;
		
		if ( checkbox.checked == true ){
		
			var atingiu_maximo = false;
			// verifica se há limite de itens que podem ser selecionados
			// verifica se limite foi ultrapassado			
			
			if ( maximo == '' )
			{
				checkbox.checked = false;
				atingiu_maximo = true;				
				alert( 'Deve ser informado a quantidade.' );
				window.close();
				maximoCampo.focus();
			}
			
			if ( maximo != 0 && qtdLinha >= maximo )
			{
				checkbox.checked = false;
				atingiu_maximo = true;
				alert( 'A quantidade máxima de itens que podem ser selecionados é ' + maximo + '.' );
				window.close();
				maximoCampo.focus();
			}			
			
			if ( atingiu_maximo == false )
			{
				itens_selecionados++;
				
				
				var nrLinha = tabela.rows.length;
				
				linha = tabela.insertRow( tabela.rows.length );
				
				tbody.appendChild( linha );
				
				linha.setAttribute( "id", "linha_" + cod )
				linha.setAttribute( "style", "background: #fff;" )
				
				var arees = document.createElement( "input" );
				arees.setAttribute( "type", "hidden" );
				arees.setAttribute( "name", "arees[]" );
				arees.setAttribute( "value", cod );
				
				var eesentescola = document.createElement( "input" );
				eesentescola.setAttribute( "type", "hidden" );
				eesentescola.setAttribute( "name", "eesentescola_" + cod );
				eesentescola.setAttribute( "value", cod );
								
				celula1 = linha.insertCell( 0 );
				celula1.innerHTML = descricao;
				celula1.appendChild( arees );
				celula1.appendChild( eesentescola );

				var eesentqtd = document.createElement( "input" );
				eesentqtd.setAttribute( "type", "text" );
				eesentqtd.setAttribute( "name", "eesentqtd_" + cod );
				eesentqtd.setAttribute( "size", "4" );
				eesentqtd.setAttribute( "style", "width: 7ex;" );
				eesentqtd.className = "normal";
				
				if(window.addEventListener){ // Mozilla, Netscape, Firefox
					eesentqtd.setAttribute( "onblur", "MouseBlur(this);" );
					eesentqtd.setAttribute( "onmouseout", "MouseOut(this);" );
					eesentqtd.setAttribute( "onfocus", "MouseClick(this); this.select();" );
					eesentqtd.setAttribute( "onmouseover", "MouseOver(this);" );
					eesentqtd.setAttribute( "onkeyup", "this.value=mascaraglobal('[#]',this.value);" );
				}
				else{ // IE
					eesentqtd.attachEvent( "onblur", function() { eventosIE( document.getElementById( 'eesentqtd_' + cod ), "1" ); } );
					eesentqtd.attachEvent( "onmouseout", function() { eventosIE( document.getElementById( 'eesentqtd_' + cod ), "2" ); } );
					eesentqtd.attachEvent( "onfocus", function() { eventosIE( document.getElementById( 'eesentqtd_' + cod ), "3" ); } );
					eesentqtd.attachEvent( "onmouseover", function() { eventosIE( document.getElementById( 'eesentqtd_' + cod ), "4" ); } );
					eesentqtd.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'eesentqtd_' + cod ), "5" ); } );
				}
				
				celula2 = linha.insertCell( 1 );
				celula2.appendChild( eesentqtd );
				celula2.style.textAlign = 'center';
			}
		}
		else{
			if ( itens_selecionados > 0 )
			{
				itens_selecionados--;
			}
			
			var linha = window.opener.document.getElementById( "linha_" + cod );
			
			tabela.deleteRow( linha.rowIndex );
		}
		
		var arChecks = document.getElementsByName( "entid[]" );
		
		var stEntid = '';
		for( i=0; i<arChecks.length; i++ ){
		
			if( arChecks[i].checked ){
				stEntid += "&arChecks[]="+arChecks[i].value;
			}
		}
		
        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: stEntid+'&req=gravarSessao',
                                       onComplete: function(res)
                                       {
                                           //alert( res );
                                       }
                                   });		
		
		alert( stEntid  );
		
	}
	
	function eventosIE( obj, tipo ){
		
		switch( tipo ){
			case('1'): MouseBlur(obj); break;
			case('2'): MouseOut(obj); break;
			case('3'): MouseClick(obj); obj.select(); break;
			case('4'): MouseOver(obj); break;
			case('5'): obj.value=mascaraglobal('[#]',obj.value); break;
		}
	}

<?php

foreach ($arEntid as $ent) {
	$arEntidadesSelecionadas = $arEntidadesSelecionadas ? $arEntidadesSelecionadas : array();
    foreach ($arEntidadesSelecionadas as $check) {
        if ($ent['codigo'] == $check && $ent['codigo']){
        	echo '$("entid_' .trim($check). '").setAttribute("checked","checked");';
        }
    }
}

?>

    $('loader-container').hide();
  -->
  </script>
</html>