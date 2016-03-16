<?php

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'reuni/www/funcoes.php';

if(isset($_REQUEST['unicod']))
	$unicod = $_REQUEST['unicod'];
else
	die('parametro necessário nao foi definido para entrar na página');

?>
<br/>
<?

$db->cria_aba( $abacod_tela, $url, "&unicod=" . $unicod );
$titulo_modulo='Bolsas de Pós-Graduação';
monta_titulo($titulo_modulo,' ');

// VARIÁVEIS
$itens_financeiro = array();
$itens_financeiro_despesa = array();

// VARIAVEL PARA BLOQUEIO DE LANÇAMENTO DE BOLSAS DE POS-GRADUAÇÃO
// S = DISPONIVEL e N = NÃO DISPONIVEL
if (strtotime(date("Y/m/d")) >= strtotime('2012/09/14')) {
	$bloqueiadistribuicaobolsas = 'N';
}
else{
	$bloqueiadistribuicaobolsas = 'S';
}

if(!isset($unidsc))
	$unidsc = nomeUnidade($unicod);

?>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<script type="text/javascript" src="/includes/abas.js"></script>
<script type="text/javascript">

    /*
       aumenta o tamanho do option dos cargos...
    */
    
    function setWidthCombo(idObjeto,size) {
       var campo =  document.getElementById(idObjeto);
       
       //campo.style==;
    
       for (var i = 0; i< campo.options.length; i++) 
       {
           campo.options[i].style.width = size;
       }
         /* */
    }
    /*
        PEGA VALOR DE UM ELEMENTO DO XML..
    */
    
    function getElementText(tag, parentElem, index) {
        if (!index) {
        index = 0;
        }
    
        var result = parentElem.responseXML.getElementsByTagName(tag)[index];
    
        if (result) {
        if (result.childNodes.length == 0) {
            return '';
        } else if (result.childNodes.length > 1) {
            return result.childNodes[1].nodeValue;
        } else {
            return result.firstChild.nodeValue;
        }
        } else {
        return '';
        }
    }	

	/*
    *    CHAMA CONTROLE PARA CARREGAR VALORES LANÇADOS.
    */
	function carregaLancadoCurso(nlcid,limctpreg,trLancado)
	{

		if(limctpreg==0 || limctpreg==null || trLancado==null || trLancado=="")
			return false;

		
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCursos.php';
       
        //var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';
		
		var req = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
		req.open(                                         
			'GET',
			 url +
				'?metodo=carregarSaldoLancadoCurso' +	
				'&unicod=<? echo $unicod  ?>' +
				'&nlcid='+nlcid+
				'&limtpreg='+limctpreg,
				true
		);
		req.onreadystatechange = function(){ respostaLancadoCurso( req, limctpreg, trLancado) };
		req.send( null );
	}

    /*
    * CARREGA VALORES LANÇADOS -- RODAPE!
    */
	function respostaLancadoCurso( req, limctpreg, trLancado )
	{
		mostraMensagemAguarde();
		if ( req.readyState != 4 )
		{
			return;
		}
		if ( req.status == 200 )
		{
			var qtde = req.responseXML.getElementsByTagName("resultado");
			var tabela = document.getElementById(trLancado);
			
			while(tabela.rows.length != 0)
			{ 
				tabela.deleteRow(0); 
			}          
				
			for (var i = 0; i < qtde.length; i++) 
			{

				var id_novo_item = Math.floor(Math.random());
				var id = 'item_' +id_novo_item;
				
				// FIM define dados gerais do item a ser adicionado
				// cria nova linha para a tabela de financeiro
				var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
				    nova_linha.id = getElementText('limctpreg', req, i);

				// FIM cria nova linha para a tabela 
				// cria as célular da nova linha
				var celula_campus	     = nova_linha.insertCell( 0 );
				var celula_2008		     = nova_linha.insertCell( 1 );
				var celula_2009		     = nova_linha.insertCell( 2 );
				var celula_2010		     = nova_linha.insertCell( 3 );
				var celula_2011		     = nova_linha.insertCell( 4 );
				var celula_2012		     = nova_linha.insertCell( 5 );
				var celula_acao_alterar  = nova_linha.insertCell( 6 );
				var celula_acao_remover  = nova_linha.insertCell( 7 );
				
				celula_campus.style.textAlign = 'right';
                		celula_campus.style.color = 'red';
                		celula_campus.style.fontWeight = 'bold';
                
                
				celula_2008.style.width = '80';
				celula_2008.style.textAlign = 'right';
				celula_2008.style.color = 'red';
				celula_2008.style.fontWeight = 'bold';
                
                
				celula_2009.style.width = '80';
				celula_2009.style.textAlign = 'right';
				celula_2009.style.color = 'red';
				celula_2009.style.fontWeight = 'bold';
                
                
				celula_2010.style.width = '80';
				celula_2010.style.textAlign = 'right';
				celula_2010.style.color = 'red';
				celula_2010.style.fontWeight = 'bold';
                
                
				celula_2011.style.width = '80';
				celula_2011.style.textAlign = 'right';
				celula_2011.style.color = 'red';
				celula_2011.style.fontWeight = 'bold';
                
                
				celula_2012.style.width = '80';
				celula_2012.style.textAlign = 'right';
				celula_2012.style.color = 'red';
				celula_2012.style.fontWeight = 'bold';
                
                
				//celula_acao_alterar.style.textAlign = 'center';
				celula_acao_alterar.style.width = '43';
				
				//celula_acao_remover.style.textAlign = 'center';
				celula_acao_remover.style.width = '46';
				
				celula_campus.id = id + '_valor';
				celula_2008.id   = id + '_valor';
				celula_2009.id   = id + '_valor';
				celula_2010.id   = id + '_valor';
				celula_2011.id   = id + '_valor';
				celula_2012.id   = id + '_valor';
				
				// FIM cria as células da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_campus		= defaultCriarInPut( getElementText('cauid', req, i) , 'campus' );
				var input_hidden_2008		= defaultCriarInPut( getElementText('lan2008', req, i) , '2008'   );
				var input_hidden_2009		= defaultCriarInPut( getElementText('lan2008', req, i) , '2009'   );
				var input_hidden_2010		= defaultCriarInPut( getElementText('lan2008', req, i) , '2010'   );
				var input_hidden_2011		= defaultCriarInPut( getElementText('lan2008', req, i) , '2011'   );
				var input_hidden_2012		= defaultCriarInPut( getElementText('lan2008', req, i) , '2012'   );
			
				celula_campus.appendChild( document.createTextNode( 'Lançado' ) );
				celula_campus.appendChild( input_hidden_campus );
				
				celula_2008.appendChild( document.createTextNode(  getElementText('lan2008', req, i) ) );			
				celula_2008.appendChild( input_hidden_2008 );
				
				celula_2009.appendChild( document.createTextNode(  getElementText('lan2009', req, i) ) );
				celula_2009.appendChild( input_hidden_2009 );
				
				celula_2010.appendChild( document.createTextNode(  getElementText('lan2010', req, i) ) );
				celula_2010.appendChild( input_hidden_2010 );
				
				celula_2011.appendChild( document.createTextNode(  getElementText('lan2011', req, i) ) );
				celula_2011.appendChild( input_hidden_2011 );
				
				celula_2012.appendChild( document.createTextNode(  getElementText('lan2012', req, i) ) );
				celula_2012.appendChild( input_hidden_2012 );
			}
		}
		else
		{
			alert( 'Erro:' + "Erro na Requisição." + req.responseText);
		}
		escondeMensagemAguarde();
		req = null;
	}
    

    /*
    * EXCLUINDO LANÇAMENTOS
    */

    //carrega Valores lançados para todos os níveis técnicos.
    function excluirRegistro(culid, tbodyidListagem)
    {
        if(culid==0 || culid==null || tbodyidListagem==null || tbodyidListagem=="")
            return;
    
        
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCursos.php';
       
      //  var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';
		

        var requisicao = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
        requisicao.open(                                         
                'GET',
                 url +
                '?metodo=excluirLancamento' + 
                '&culid='+culid,
                true
        );
        requisicao.onreadystatechange = function(){ respostaExcluirRegistro( requisicao, culid, tbodyidListagem ) };
        requisicao.send( null );
        
        window.location.reload();			
    }

    /*
    * CALLBACK DA FUNÇÃO DE REMOÇÃO -//PARA RECARREGAR A TELA.
    */
    function respostaExcluirRegistro(req, culid , tbodyidListagem)
    {
        
        if ( req.readyState != 4 )
        {
            return;
        }
        
        if ( req.status == 200 )
        {
            alert('Registro Excluído com êxito!');

            if(tbodyidListagem=='tabela_mestrado')
                carregaTelaMestrado();
	    /*
            if(tbodyidListagem=='tabela_tece')
                carregaTelaTecE();

            if(tbodyidListagem=='tabela_tecbcd')
                carregaTelaTecBCD();
	    */
        }
        else
        {
            alert( 'Erro:' + "Erro ao excluir ao registro"+ req.responseText);
        }
        req = null;	
    }

	/*
	* CHAMA CONTROLE PARA VALORES DISPONÍVEIS
	*/
	function carregaDisponivelCurso(nlcid,limtpreg,trDisponivel)
	{

		if(limtpreg==0 || limtpreg==null || trDisponivel==null || trDisponivel=="")
			return false;

		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCursos.php';
		//var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';

		var requisicao = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
		requisicao.open(                                         
			'GET',
			 url +
				'?metodo=carregarSaldoDisponivelCurso' +	
				'&unicod=<? echo $unicod  ?>' +
				'&nlcid='+nlcid +
				'&limtpreg='+limtpreg,
				true
		);
		requisicao.onreadystatechange = function(){ respostaDisponivelCurso( requisicao, limtpreg, trDisponivel ) };
		requisicao.send( null );			
	}

    /*
   * CALLBACK PARA PREENCHER O VALOR DO SALDO DISPONÍVEL
   */
	function respostaDisponivelCurso( req, limtpreg, trDisponivel )
	{
		
		mostraMensagemAguarde();
        
		if ( req.readyState != 4 )
		{
			return;
		}
		if ( req.status == 200 )
		{
			//var tr = document.getElementById( trDisponivel );
			var qtde = req.responseXML.getElementsByTagName("resultado");
			var tabela = document.getElementById(trDisponivel);
		    
			while(tabela.rows.length > 0)
			{ 
				tabela.deleteRow(0); 
			} 
            
			for (var i = 0; i < qtde.length; i++) 
			{

				//var id_novo_item = tabela.rows.length;	//pega o tamanho da tabela :D
				var id_novo_item = Math.floor(Math.random());
				var id = 'item_' +id_novo_item;

				// item_docente_adicionados[id_novo_item] = id;
				
				// FIM define dados gerais do item a ser adicionado
				// cria nova linha para a tabela de financeiro
				var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
				    nova_linha.id = getElementText('nlcid', req, i);
				// nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
				// FIM cria nova linha para a tabela 
				// cria as célular da nova linha
				var celula_campus	 = nova_linha.insertCell(0);
				var celula_2008		 = nova_linha.insertCell(1);
				var celula_2009		 = nova_linha.insertCell(2);
				var celula_2010		 = nova_linha.insertCell(3);
				var celula_2011		 = nova_linha.insertCell(4);
				var celula_2012		 = nova_linha.insertCell(5);
				var celula_acao_alterar  = nova_linha.insertCell(6);
				var celula_acao_remover  = nova_linha.insertCell(7);
				
				celula_campus.style.textAlign = 'right';
				celula_campus.style.color = 'blue';
				celula_campus.style.fontWeight = 'bold';

				celula_2008.style.width = '80';
				celula_2008.style.textAlign = 'right';
				celula_2008.style.color = 'blue';
				celula_2008.style.fontWeight = 'bold';
				
				celula_2009.style.width = '80';
				celula_2009.style.color = 'blue';
				celula_2009.style.fontWeight = 'bold';
                
				celula_2009.style.textAlign = 'right';
				celula_2009.style.color = 'blue';
				celula_2009.style.fontWeight = 'bold';
				
				celula_2010.style.width = '80';
				celula_2010.style.textAlign = 'right';
				celula_2010.style.color = 'blue';
				celula_2010.style.fontWeight = 'bold';
				
				celula_2011.style.width = '80';
				celula_2011.style.textAlign = 'right';
				celula_2011.style.color = 'blue';
				celula_2011.style.fontWeight = 'bold';

				
				celula_2012.style.width = '80';
				celula_2012.style.textAlign = 'right';
				celula_2012.style.color = 'blue';
				celula_2012.style.fontWeight = 'bold';
					
				//celula_acao_alterar.style.textAlign = 'center';
				celula_acao_alterar.style.width = '43';
				
				//celula_acao_remover.style.textAlign = 'center';
				celula_acao_remover.style.width = '46';
				
				celula_campus.id = id + '_valor';
				celula_2008.id   = id + '_valor';
				celula_2009.id   = id + '_valor';
				celula_2010.id   = id + '_valor';
				celula_2011.id   = id + '_valor';
				celula_2012.id   = id + '_valor';
				
				// FIM cria as células da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_campus	= defaultCriarInPut(  getElementText('cauid', req, i) , 'campus' );
				var input_hidden_2008	= defaultCriarInPut(  getElementText('lan2008', req, i) , '2008'   );
				var input_hidden_2009	= defaultCriarInPut(  getElementText('lan2008', req, i) , '2009'   );
				var input_hidden_2010	= defaultCriarInPut(  getElementText('lan2008', req, i) , '2010'   );
				var input_hidden_2011	= defaultCriarInPut(  getElementText('lan2008', req, i) , '2011'   );
				var input_hidden_2012	= defaultCriarInPut(  getElementText('lan2008', req, i) , '2012'   );
				
				celula_campus.appendChild( document.createTextNode( 'Disponivel' ) );
				celula_campus.appendChild( input_hidden_campus );
				
				celula_2008.appendChild( document.createTextNode(  getElementText('lan2008', req, i) ) );			
				celula_2008.appendChild( input_hidden_2008 );
				
				celula_2009.appendChild( document.createTextNode(  getElementText('lan2009', req, i) ) );
				celula_2009.appendChild( input_hidden_2009 );
				
				celula_2010.appendChild( document.createTextNode(  getElementText('lan2010', req, i) ) );
				celula_2010.appendChild( input_hidden_2010 );
				
				celula_2011.appendChild( document.createTextNode(  getElementText('lan2011', req, i) ) );
				celula_2011.appendChild( input_hidden_2011 );
				
				celula_2012.appendChild( document.createTextNode(  getElementText('lan2012', req, i) ) );
				celula_2012.appendChild( input_hidden_2012 );

			}

		}
		else
		{
			alert( 'Erro:' + "Erro na Requisição." + req.responseText);
		}
        escondeMensagemAguarde();
		req = null;		
	}

	
		
	/**
	 * Cria o conteúdo de uma célula da tabela
	 * 
	 * @param string
	 * @param string
	 * @return object
	 */
	function defaultCriarInPut( valor, nome_campo )
	{
		var input = document.createElement( 'input' );
		input.type = 'hidden';
		input.name = nome_campo + '[]';
        input.id = nome_campo;
		input.value = valor;
		return input;
	}
	

	/**
	 * Organiza as cores da linhas, ora com valor vazio ora com 'efefef'.
	 * 
	 * @return void
	 */
	function corSimcorNao(table)
	{
		var tabela = document.getElementById( table );
		var quantidade_tr = tabela.rows.length;
		var cor = '';
		for ( var i = 0; i < quantidade_tr; i++ )
		{
			var tr = tabela.rows[i];
			cor = cor == '' ? '#e9e9e9' : '' ;
			tr.style.backgroundColor = cor;
			tr.onmouseout = new Function( ' this.style.backgroundColor = "' + cor + '"; ' );
		}
	}


    function defineCorItemAlterado(idRow, tbodyidListagem)
    {
        corSimcorNao(tbodyidListagem);
        
        var tabela = document.getElementById(tbodyidListagem);

        for ( var i = 0; i < tabela.rows.length; i++ )
        {   
            //alert('verificando:' + tabela.rows[i].id + ' igual? ' + idRow );
           if(tabela.rows[i].id == idRow )
           {
               //tabela.rows[i].onmouseover           = '';
               tabela.rows[i].onmouseover           = new Function( ' this.style.backgroundColor = "#ffffaa"; ' );
               tabela.rows[i].onmouseout            = new Function( ' this.style.backgroundColor = "#ffffaa"; ' );
               tabela.rows[i].style.backgroundColor = '#ffffaa';
           }
        }
    }
    
    function escondeMensagemAguarde()
    {
        document.getElementById('aguarde').style.visibility = 'hidden';
		document.getElementById('aguarde').style.display = 'none';
    }
    
    function mostraMensagemAguarde()
    {
        document.getElementById('aguarde').style.visibility = 'visible';
		document.getElementById('aguarde').style.display = 'block';
    }
</script>


	<table width="100%" cellpadding="0" cellspacing="0">
		<tr>
			<td>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td width="20%" align="right" class="SubTituloDireita">Código:</td>
						<td width="80%"><? echo $unicod; ?></td>
					</tr>
					<tr>
						<td width="20%" align="right" class="SubTituloDireita">Nome da
						Instituição:</td>
						<td width="80%"><? echo $unidsc; ?></td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
	
	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Mestrado</li>
			</ul>
		</div>
		<!-- FINANCEIRO -->
		<div id="aba_docente" class="conteudoAba">
			<?
			   include('planoDistribuicaoCursoMestrado.inc');
			?>
		</div>
	</div>

	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Doutorado</li>
			</ul>
		</div>
		<!-- FINANCEIRO -->
		<div id="aba_tece" class="conteudoAba">
			<?
				include('planoDistribuicaoCursoDoutorado.inc');
			?>
		</div>
	</div>

	
	<div id="abas" style="display: block;">
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Pós-Doutorado</li>
			</ul>
		</div>
		<!-- FINANCEIRO -->
		<div id="aba_tece" class="conteudoAba">
			<?
				include('planoDistribuicaoCursoPosDoutorado.inc');
			?>
		</div>
	</div>

    <script type="text/javascript">
              
    </script>

    
