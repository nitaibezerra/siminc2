
<table width="90%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td            valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: left;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Curso</strong></td>
			<td width="80" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2008</strong></td>
			<td width="80" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2009</strong></td>
			<td width="80" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2010</strong></td>
			<td width="80" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2011</strong></td>
			<td width="80" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: right;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>2012</strong></td>
			<td width="100" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: center;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
		</tr>
	</thead>
	<tbody>
		<tr bgcolor="#f7f7f7">
			<td>
			<?
				$sql = "select curid as codigo, curnome as descricao from reuni.solicitacaocurso where limtpreg = 'M' and unicod ='". $unicod."' ORDER BY curnome ";
				//monta_combo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='',$size='',$obrig='', $id = '')
				echo $db->monta_combo('mestrado_curid',$sql,$bloqueiadistribuicaobolsas,'','','','','100%','N', 'mestrado_curid');

			?>
			</td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'mestrado_2008', 'N', 'N', '2008', 10, 20, '####', '', 'right', '', 0, ' id="mestrado_2008" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'mestrado_2009', 'N', 'N', '2008', 10, 20, '####', '', 'right', '', 0, ' id="mestrado_2009" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'mestrado_2010', 'N', $bloqueiadistribuicaobolsas, '2008', 10, 20, '####', '', 'right', '', 0, ' id="mestrado_2010" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'mestrado_2011', 'N', $bloqueiadistribuicaobolsas, '2008', 10, 20, '####', '', 'right', '', 0, ' id="mestrado_2011" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'mestrado_2012', 'N', $bloqueiadistribuicaobolsas, '2008', 10, 20, '####', '', 'right', '', 0, ' id="mestrado_2012" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: left;">&nbsp

			<?php if($bloqueiadistribuicaobolsas == 'S'){ ?>
			<!--  comentando botão incluir   -->
				 <a href="javascript:adicionarRegistroMestrado();" title="Incluir" style="margin: 0; padding: 0 ;">
					<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
				</a> 
			<?php } ?>				
			</td>
		</tr>
		<tr>
			<td colspan="7">					
				<div class="div_rolagem" id="div_tabela_docente" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
					<table id="tabela_mestrado" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tr valign="bottom">
							<td style="height: 0; margin: 0; padding: 0;"></td>
							<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="36" style="height: 0; margin: 0; padding: 0;"></td>
							<td width="46" style="height: 0; margin: 0; padding: 0;"></td>
						</tr>
					</table>
				</div>
			</td>
		</tr>	
		<tr valign="bottom">
			<td align="right" colspan="7" style="border-top: 3px solid #dfdfdf;">
				<table id="tabela_mestrado_lancado" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
					<tr valign="bottom">
						<td style="height: 0; margin: 0; padding: 0; text-align: right;"></td>
						<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="36" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="60" style="height: 0; margin: 0; padding: 0;"></td>
					</tr>
				</table>
			</td>
		</tr>	
		<tr valign="bottom">
			<td align="right" colspan="7" style="border-top: 3px solid #dfdfdf;">
				<table id="tabela_mestrado_disponivel" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
					<tr valign="bottom">
						<td style="height: 0; margin: 0; padding: 0; text-align: right;"></td>
						<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="85" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="83" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="36" style="height: 0; margin: 0; padding: 0;"></td>
						<td width="60" style="height: 0; margin: 0; padding: 0;"></td>
					</tr>
				</table>
			</td>
		</tr>		
	</tbody>
</table>
<br>

<script type="text/javascript">

//
	function carregaListagemMestrado(nlcid,limtpreg,tabela)
	{
		if(limtpreg==0 || limtpreg==null || tabela==null || tabela=="")
			return false;
	
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCursos.php';
       
         //var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';
		
		var requisicao = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

		requisicao.open(                                         
			'GET',
			 url +
				'?metodo=carregaListaLancamentos' + 
				'&tbody='+tabela+
				'&unicod=<? echo $unicod  ?>' +
				'&xml=true' +
				'&limtpreg='+limtpreg +
				'&nlcid=' + nlcid,
				true
		);
		
		requisicao.onreadystatechange = function(){ respostaCarregaListagemMestrado( requisicao, limtpreg, tabela ) };
		requisicao.send( null );
	}

    
	function respostaCarregaListagemMestrado(req, limtpreg, tbodyidListagem )
	{
		if ( req.readyState != 4 )
		{
		    mostraMensagemAguarde();
		    return;
		}
		if ( req.status == 200 )
		{
			eval("var listagemMestrado = " + req.responseText + ";");
			top.listagemMestrado = listagemMestrado;
			
			//limpando a tabela.
			var tabela = document.getElementById(tbodyidListagem);
			
			while(tabela.rows.length > 0)
			{ 
				tabela.deleteRow(0); 
			} 
			
			if(top.listagemMestrado.length == 0)
			    return;
			
			/* Para que os elementos... apareçam na ordem que chega.  */
			var id_novo_item = Math.floor(Math.random());
			var id = 'item_x' +id_novo_item;
			var nova_linha = tabela.insertRow( tabela.rows.length - 1 );

			for (var i = 0; i < listagemMestrado.length; i++)
			{
				top.listagemMestrado[i]['tabela'] = tbodyidListagem;
                 

				var id_novo_item = Math.floor(Math.random());
				var id = 'item_' +id_novo_item+  listagemMestrado[i]['calid'];

				// FIM define dados gerais do item a ser adicionado
				// cria nova linha para a tabela de financeiro
				var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
				nova_linha.id =   tabela.rows.length+1;
				    //nova_linha.id = getElementText('calid', req, i);
                    //nova_linha.id = listagemMestrado[i]['calid'];
				    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
                   // nova_linha.onmouseover = new Function( ' alert(this.id); ' );
				// FIM cria nova linha para a tabela 
				// cria as célular da nova linha
				var celula_campus	 = nova_linha.insertCell( 0 );
				var celula_2008		 = nova_linha.insertCell( 1 );
				var celula_2009		 = nova_linha.insertCell( 2 );
				var celula_2010		 = nova_linha.insertCell( 3 );
				var celula_2011		 = nova_linha.insertCell( 4 );
				var celula_2012		 = nova_linha.insertCell( 5 );
				var celula_acao_alterar  = nova_linha.insertCell( 6 );
				var celula_acao_remover  = nova_linha.insertCell( 7 );
				
				celula_campus.style.textAlign = 'left';
		
				celula_2008.style.width = '80';
				celula_2008.style.textAlign = 'right';
				
				celula_2009.style.width = '80';
				celula_2009.style.textAlign = 'right';
				
				celula_2010.style.width = '80';
				celula_2010.style.textAlign = 'right';
				
				celula_2011.style.width = '80';
				celula_2011.style.textAlign = 'right';
				
				celula_2012.style.width = '80';
				celula_2012.style.textAlign = 'right';
				
				//celula_acao_alterar.style.textAlign = 'center';
				celula_acao_alterar.style.width = '30';
				
				//celula_acao_remover.style.textAlign = 'center';
				celula_acao_remover.style.width = '43';
				
				celula_campus.id = id + '_valor';
				celula_2008.id   = id + '_valor';
				celula_2009.id   = id + '_valor';
				celula_2010.id   = id + '_valor';
				celula_2011.id   = id + '_valor';
				celula_2012.id   = id + '_valor';
				
				// FIM cria as células da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_campus		= defaultCriarInPut( listagemMestrado[i]['curnome']   , 'form_campus' );
				var input_hidden_2008		= defaultCriarInPut( listagemMestrado[i]['lan2008']   , 'form_2008'   );
				var input_hidden_2009		= defaultCriarInPut( listagemMestrado[i]['lan2009']   , 'form_2009'   );
				var input_hidden_2010		= defaultCriarInPut( listagemMestrado[i]['lan2010']   , 'form_2010'   );
				var input_hidden_2011		= defaultCriarInPut( listagemMestrado[i]['lan2011']   , 'form_2011'   );
				var input_hidden_2012		= defaultCriarInPut( listagemMestrado[i]['lan2012']   , 'form_2012'   );

				// FIM cria campos ocultos com os dados do novo item
				// define dado para as células de ação
				var link_acao_alterar = document.createElement( 'a' );
				   link_acao_alterar.href = 'javascript:editarRegistroMestrado(' + nova_linha.id                     + ','
                                                                                + listagemMestrado[i]['culid']   + ','
                                                                                + listagemMestrado[i]['curid']   + ','
                                                                                + listagemMestrado[i]['lan2008'] + ','
                                                                                + listagemMestrado[i]['lan2009'] + ','
                                                                                + listagemMestrado[i]['lan2010'] + ','
                                                                                + listagemMestrado[i]['lan2011'] + ','
                                                                               + listagemMestrado[i]['lan2012'] + ');';
				<?php if($bloqueiadistribuicaobolsas == 'S'){ ?>
					//--- comentando botões de edição                                                                                
				    link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '&nbsp&nbsp<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				<?php } ?>
				    
				var link_acao_remover = document.createElement( 'a' ); //vai criar o danado do <a href!>//
				    link_acao_remover.href = 'javascript:excluirRegistro(listagemMestrado['+i+'][\'culid\'], listagemMestrado['+i+'][\'tabela\']);';//'

				<?php if($bloqueiadistribuicaobolsas == 'S'){ ?>
					//--- comentando botões de exclusao                                                                                
					link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif"  border="0" align="absmiddle"/>';
				<?php } ?>
			
				link_acao_alterar.id = id + '_ancora';
			
				celula_campus.appendChild( document.createTextNode( listagemMestrado[i]['curnome']  ) );
				celula_campus.appendChild( input_hidden_campus );
				
				celula_2008.appendChild( document.createTextNode(   listagemMestrado[i]['lan2008'] ) );			
				celula_2008.appendChild( input_hidden_2008 );
				
				celula_2009.appendChild( document.createTextNode(   listagemMestrado[i]['lan2009'] ) );
				celula_2009.appendChild( input_hidden_2009 );     
				                                                  
				celula_2010.appendChild( document.createTextNode(   listagemMestrado[i]['lan2010'] ) );
				celula_2010.appendChild( input_hidden_2010 );      
				                                                   
				celula_2011.appendChild( document.createTextNode(   listagemMestrado[i]['lan2011'] ) );
				celula_2011.appendChild( input_hidden_2011 );      
				                                                   
				celula_2012.appendChild( document.createTextNode(   listagemMestrado[i]['lan2012'] ) );
				celula_2012.appendChild( input_hidden_2012 );
				
				if ( link_acao_alterar != null  )
					celula_acao_alterar.appendChild( link_acao_alterar );

				if ( link_acao_remover != null  )
					celula_acao_remover.appendChild( link_acao_remover );

				// FIM altera soma dos valores de acordo com o novo item
				// atualiza as cores alternadas das linhas
				corSimcorNao(tbodyidListagem);
			}
            escondeMensagemAguarde();            
		}
		else
		{
			alert( 'Erro:' + "Erro na Requisição.");
		}			
		req = null;	
	}


	/**
	 * Insere lançamento da distribuição para docente.
	 * 
	 * @return void
	 */
	function adicionarRegistroMestrado()
	{
		// captura os campos do financeiro
		var mestrado_curid 	= document.getElementById('mestrado_curid');
		var culid           	= mestrado_curid.getAttribute('culid');

		var mestrado_2008 	= document.getElementById('mestrado_2008');
		var mestrado_2009	= document.getElementById('mestrado_2009');
		var mestrado_2010 	= document.getElementById('mestrado_2010');
		var mestrado_2011	= document.getElementById('mestrado_2011');
		var mestrado_2012 	= document.getElementById('mestrado_2012');

		if( trim(mestrado_2008.value)  == '' && 
		    trim(mestrado_2009.value)  == '' &&
		    trim(mestrado_2010.value)  == '' &&
		    trim(mestrado_2011.value)  == '' &&
		    trim(mestrado_2012.value)  == '' )
		{
		    alert('Não é possível salvar todos os valores igual a 0 (Zero).');
		    return;
		}
	
	
		if(	trim(mestrado_2008.value)  == '')
			     mestrado_2008.value = 0;
		if(	trim(mestrado_2009.value)  == '')
			     mestrado_2009.value = 0;
		if(	trim(mestrado_2010.value)  == '')
			     mestrado_2010.value = 0;
		if(	trim(mestrado_2011.value)  =='')
			     mestrado_2011.value = 0;
		if(	trim(mestrado_2012.value)  == '')
			     mestrado_2012.value = 0;
	
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCursos.php';
	
		var req = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
		req.open(
		    'GET',
		     url +
			'?metodo=salvarLancamentoCurso' +
			'&unicod=<? echo $unicod  ?>'    +
			'&limtpreg=M'+
			'&nlcid=4'+
			'&curid='+mestrado_curid.value   +
			'&culid='+culid   +
			'&lan2008='+mestrado_2008.value+
			'&lan2009='+mestrado_2009.value+
			'&lan2010='+mestrado_2010.value+
			'&lan2011='+mestrado_2011.value+
			'&lan2012='+mestrado_2012.value,
			true
		);
	
		limparCamposMestrado();
	
		
		req.onreadystatechange = function(){ respostaAdicionarRegistroMestrado(req); };
		req.send( null );        
		
        
		//document.getElementById( 'docente_cauid' ).focus();
	}	

	function respostaAdicionarRegistroMestrado(req)
	{
		if ( req.readyState != 4 )
		{
		    return;
		}
        
		if ( req.status == 200 || req.status == 500  )
		{
		    if(req.status == 500)
			alert("Observação:" + req.responseText);
		    
		    carregaTelaMestrado();
		}
		else
		{
		    escondeMensagemAguarde();
		    alert('erro:' + req.responseText);
		}
		///   alert('status:' + req.status);
	}


	/**
	 * Prepara formulário para edição de um item da tabela financeiro.
	 *                                                           
	 */
	function editarRegistroMestrado(linhaTabela,culid,campus,campo_2008,campo_2009,campo_2010,campo_2011,campo_2012)
	{

        
		var form_campus	        = document.getElementById( 'mestrado_curid' );
		var form_2008		= document.getElementById( 'mestrado_2008' );
		var form_2009		= document.getElementById( 'mestrado_2009' );
		var form_2010		= document.getElementById( 'mestrado_2010' );
		var form_2011		= document.getElementById( 'mestrado_2011' );
		var form_2012		= document.getElementById( 'mestrado_2012' );
        
		//limpando os campos primeiro...
		limparCamposMestrado();
		
		form_2008.value 	= campo_2008;
		form_2009.value 	= campo_2009;
		form_2010.value 	= campo_2010;
		form_2011.value 	= campo_2011;
		form_2012.value 	= campo_2012;

		for (var i =0; i < form_campus.options.length; i++)
		{
		    if (form_campus.options[i].getAttribute('value') == campus)
		    {
			form_campus.options[i].selected = true;
			form_campus.setAttribute('culid', culid);
		    }
		}
	       // form_campus.
		defineCorItemAlterado(linhaTabela, 'tabela_mestrado');
		
	}

    function limparCamposMestrado()
    {
        var form_campus	    	= document.getElementById( 'mestrado_curid' );
	var form_2008		= document.getElementById( 'mestrado_2008' );
	var form_2009		= document.getElementById( 'mestrado_2009' );
	var form_2010		= document.getElementById( 'mestrado_2010' );
	var form_2011		= document.getElementById( 'mestrado_2011' );
	var form_2012		= document.getElementById( 'mestrado_2012' );
            
        form_campus.setAttribute('culid', null);
        form_2008.value = '';
        form_2009.value = '';
        form_2010.value = '';
        form_2011.value = '';
        form_2012.value = '';
        
        corSimcorNao('tabela_mestrado');
    }
    
	function carregaTelaMestrado()
	{
		carregaListagemMestrado("4","M","tabela_mestrado");
		carregaDisponivelCurso("4","M","tabela_mestrado_disponivel");
		carregaLancadoCurso("4","M","tabela_mestrado_lancado");
	}	
    
    carregaTelaMestrado();
    
	 

</script>




