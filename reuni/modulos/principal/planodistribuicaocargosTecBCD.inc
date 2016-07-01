
<table width="90%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td            valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: left;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Campus Universitário</strong></td>
            <td            valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;  text-align: left;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Cargo</strong></td>
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
				$sql = "select cauid as codigo, caudesc as descricao from reuni.campusuniversitario where unicod ='". $unicod. "' order by caudesc";
				//monta_combo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='',$size='',$obrig='', $id = '')
				echo $db->monta_combo('tecbcd_cauid',$sql,'S','','','','','','N', 'tecbcd_cauid');
                ?>
			</td>
			<td>
        
                <?
                $sql = "
                SELECT cargo.carcod as codigo, cargo.cardesc as descricao FROM	reuni.cargo	WHERE cargo.carident in ('B', 'C', 'D') order by cargo.cardesc ";
                $db->monta_combo('tecbcd_carcod',$sql,'S','','','','','','N', 'tecbcd_carcod');
                ?>
			</td>            
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'tecbcd_2008', 'N', 'S', '2008', 10, 20, '####', '', 'right', '', 0, ' id="tecbcd_2008" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'tecbcd_2009', 'N', 'S', '2008', 10, 20, '####', '', 'right', '', 0, ' id="tecbcd_2009" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'tecbcd_2010', 'N', 'S', '2008', 10, 20, '####', '', 'right', '', 0, ' id="tecbcd_2010" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'tecbcd_2011', 'N', 'S', '2008', 10, 20, '####', '', 'right', '', 0, ' id="tecbcd_2011" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: right;"><? echo campo_texto( 'tecbcd_2012', 'N', 'S', '2008', 10, 20, '####', '', 'right', '', 0, ' id="tecbcd_2012" onblur="MouseBlur(this);" ' );?></td>
			<td align="right" style=" text-align: left;">&nbsp
				<a href="javascript:adicionarRegistroTecBCD();" title="Incluir" style="margin: 0; padding: 0 ;">
					<img src="../imagens/gif_inclui.gif" border="0" align="absmiddle"/>
				</a>
			</td>
		</tr>
		<tr>
			<td colspan="8">					
				<div class="div_rolagem" id="div_tabela_docente" style="width: 100%; padding: 0; margin: 0; height: 100px;position-y: absolute;">
					<table id="tabela_tecbcd" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
						<tr valign="bottom">
							<td            style="height: 0; margin: 0; padding: 0;"></td>
                            <td            style="height: 0; margin: 0; padding: 0;"></td>
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
			<td align="right" colspan="8" style="border-top: 3px solid #dfdfdf;">
				<table id="tabela_tecbcd_lancado" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
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
			<td align="right" colspan="8" style="border-top: 3px solid #dfdfdf;">
				<table id="tabela_tecbcd_disponivel" width="100%" align="center" border="0" cellspacing="0" cellpadding="2">
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


  //  setWidthCombo('tecbcd_carcod','300px');


	function carregaListagemTecBCD(nlcid,tabela)
	{
		if(nlcid==0 || nlcid==null || tabela==null || tabela=="")
			return false;
	
		
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCargos.php';
       
      // var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';
		
		
		var requisicao = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

		requisicao.open(                                         
			'GET',
			 url +
				'?acao=carregaListaLancamentos' + 
				'&tbody='+tabela+
				'&unicod=<? echo $unicod  ?>' +
				'&xml=true' +
				'&nlcid='+nlcid,
				true
		);
		
		requisicao.onreadystatechange = function(){ respostaCarregaListagemTecBCD( requisicao, nlcid, tabela ) };
		requisicao.send( null );
	}


	function respostaCarregaListagemTecBCD(req, nlcid, tbodyidListagem )
	{
		if ( req.readyState != 4 )
		{
            mostraMensagemAguarde();
			return;
		}
		if ( req.status == 200 )
		{
            eval("var listagemTecBCD = " + req.responseText + ";");
            //conforme dito pelo DOUGLAS    ... muda de escopo ao criar um novo elemento... entaum
            //tem que definir uma var globs pra guardar o valor
            top.listagemTecBCD = listagemTecBCD;

            //limpando a tabela.
			var tabela = document.getElementById(tbodyidListagem);
            while(tabela.rows.length > 0)
            { 
                tabela.deleteRow(0); 
            } 
            
      
            /* Para que os elementos... apareçam na ordem que chega.  */
            var id_novo_item = Math.floor(Math.random());
            var id = 'item_x' +id_novo_item;
            var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
            
            
			for (var i = 0; i < listagemTecBCD.length; i++)
			{
				top.listagemTecBCD[i]['tabela'] = tbodyidListagem;
                 

                var id_novo_item = Math.floor(Math.random());
				var id = 'item_' +id_novo_item+  top.listagemTecBCD[i]['calid'];

				// FIM define dados gerais do item a ser adicionado
				// cria nova linha para a tabela de financeiro
				var nova_linha = tabela.insertRow( tabela.rows.length - 1 );
                    nova_linha.id =   tabela.rows.length+1;
				    //nova_linha.id = getElementText('calid', req, i);
                    //nova_linha.id = listagemTecBCD[i]['calid'];
				    nova_linha.onmouseover = new Function( ' this.style.backgroundColor = "#ffffcc"; ' );
				// FIM cria nova linha para a tabela 
				// cria as célular da nova linha
				var celula_campus	 = nova_linha.insertCell( 0 );
                var celula_cargo	 = nova_linha.insertCell( 1 );
				var celula_2008		 = nova_linha.insertCell( 2 );
				var celula_2009		 = nova_linha.insertCell( 3 );
				var celula_2010		 = nova_linha.insertCell( 4 );
				var celula_2011		 = nova_linha.insertCell( 5 );
				var celula_2012		 = nova_linha.insertCell( 6 );
				var celula_acao_alterar  = nova_linha.insertCell( 7 );
				var celula_acao_remover  = nova_linha.insertCell( 8 );
				
				celula_campus.style.textAlign = 'left';
                celula_cargo.style.textAlign = 'left';
		
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
                celula_cargo.id  = id + '_valor';
				celula_2008.id   = id + '_valor';
				celula_2009.id   = id + '_valor';
				celula_2010.id   = id + '_valor';
				celula_2011.id   = id + '_valor';
				celula_2012.id   = id + '_valor';
				
				// FIM cria as células da nova linha
				// cria campos ocultos com os dados do novo item
				var input_hidden_campus		= defaultCriarInPut(  listagemTecBCD[i]['caudesc'] , 'form_campus' );
                var input_hidden_cargo		= defaultCriarInPut(  listagemTecBCD[i]['cardesc'] , 'form_cargo'  );
				var input_hidden_2008		= defaultCriarInPut(  listagemTecBCD[i]['lan2008'] , 'form_2008'   );
				var input_hidden_2009		= defaultCriarInPut(  listagemTecBCD[i]['lan2009'] , 'form_2009'   );
				var input_hidden_2010		= defaultCriarInPut(  listagemTecBCD[i]['lan2010'] , 'form_2010'   );
				var input_hidden_2011		= defaultCriarInPut(  listagemTecBCD[i]['lan2011'] , 'form_2011'   );
				var input_hidden_2012		= defaultCriarInPut(  listagemTecBCD[i]['lan2012'] , 'form_2012'   );

				// FIM cria campos ocultos com os dados do novo item
				// define dado para as células de ação
				var link_acao_alterar = document.createElement( 'a' );
				   link_acao_alterar.href = 'javascript:editarRegistroTecBCD('  + nova_linha.id                    + ','
                                                                                + top.listagemTecBCD[i]['calid']   + ','
                                                                                + top.listagemTecBCD[i]['cauid']   + ','
                                                                                + top.listagemTecBCD[i]['carcod']  + ','                                                                                
                                                                                + top.listagemTecBCD[i]['lan2008'] + ','
                                                                                + top.listagemTecBCD[i]['lan2009'] + ','
                                                                                + top.listagemTecBCD[i]['lan2010'] + ','
                                                                                + top.listagemTecBCD[i]['lan2011'] + ','
                                                                                + top.listagemTecBCD[i]['lan2012'] + ');';
                   link_acao_alterar.title = 'Editar';
				    link_acao_alterar.innerHTML = '&nbsp&nbsp<img src="../imagens/alterar.gif" border="0" align="absmiddle"/>';
				    
				var link_acao_remover = document.createElement( 'a' ); //vai criar o danado do <a href!>//
				    link_acao_remover.href = 'javascript:excluirRegistro(top.listagemTecBCD['+i+'][\'calid\'], top.listagemTecBCD['+i+'][\'tabela\']);';//'
				    link_acao_remover.title = 'Excluir';
				    link_acao_remover.innerHTML = '<img src="../imagens/excluir.gif"  border="0" align="absmiddle"/>';
			
				link_acao_alterar.id = id + '_ancora';
			
				celula_campus.appendChild( document.createTextNode( listagemTecBCD[i]['caudesc']  ) );
				celula_campus.appendChild( input_hidden_campus );
				
				celula_cargo.appendChild( document.createTextNode( listagemTecBCD[i]['cardesc']  ) );
				celula_cargo.appendChild( input_hidden_cargo );
				
				celula_2008.appendChild( document.createTextNode(   listagemTecBCD[i]['lan2008'] ) );			
				celula_2008.appendChild( input_hidden_2008 );
				
				celula_2009.appendChild( document.createTextNode(   listagemTecBCD[i]['lan2009'] ) );
				celula_2009.appendChild( input_hidden_2009 );     
				                                                  
				celula_2010.appendChild( document.createTextNode(   listagemTecBCD[i]['lan2010'] ) );
				celula_2010.appendChild( input_hidden_2010 );      
				                                                   
				celula_2011.appendChild( document.createTextNode(   listagemTecBCD[i]['lan2011'] ) );
				celula_2011.appendChild( input_hidden_2011 );      
				                                                   
				celula_2012.appendChild( document.createTextNode(   listagemTecBCD[i]['lan2012'] ) );
				celula_2012.appendChild( input_hidden_2012 );
				
				if ( link_acao_alterar != null  )
					celula_acao_alterar.appendChild( link_acao_alterar );

				if ( link_acao_remover != null  )
					celula_acao_remover.appendChild( link_acao_remover );

				// FIM altera soma dos valores de acordo com o novo item
				// atualiza as cores alternadas das linhas
				corSimcorNao(tbodyidListagem);
			}

		}
		else
		{
			alert( 'Erro:' + "Erro na Requisição.");
		}
        escondeMensagemAguarde();
		req = null;	
	}

	/**
	 * Insere lançamento da distribuição para docente.
	 * 
	 * @return void
	 */
	function adicionarRegistroTecBCD()
	{
        
		var tecbcd_cauid 	= document.getElementById('tecbcd_cauid');
        var calid           = tecbcd_cauid.getAttribute('calid'); ///para o controle saber se eh insert ou update!

		var tecbcd_carcod   = document.getElementById('tecbcd_carcod');
		var tecbcd_2008 	= document.getElementById('tecbcd_2008');
		var tecbcd_2009	    = document.getElementById('tecbcd_2009');
		var tecbcd_2010 	= document.getElementById('tecbcd_2010');
		var tecbcd_2011	    = document.getElementById('tecbcd_2011');
		var tecbcd_2012 	= document.getElementById('tecbcd_2012');
        

        if(	trim(tecbcd_2008.value)  == '' && 
            trim(tecbcd_2009.value)  == '' &&
            trim(tecbcd_2010.value)  == '' &&
            trim(tecbcd_2011.value)  == '' &&
            trim(tecbcd_2012.value)  == '' )
        {
            alert('Não é possível salvar todos os valores igual a 0 (Zero).');
            return;
        }


        if(	trim(tecbcd_2008.value)  == '')
                 tecbcd_2008.value = 0;
        if(	trim(tecbcd_2009.value)  == '')
                 tecbcd_2009.value = 0;
        if(	trim(tecbcd_2010.value)  == '')
                 tecbcd_2010.value = 0;
        if(	trim(tecbcd_2011.value)  =='')
                 tecbcd_2011.value = 0;
        if(	trim(tecbcd_2012.value)  == '')
                 tecbcd_2012.value = 0;
       
		
		var url = '/<?= $_SESSION['sisdiretorio'] ?>/controleDistribuicaoCargos.php';
       
        //var url = '?modulo=principal/controleDistribuicaoCargos&acao=P';
		
        var req = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );
        req.open(                                         
            'GET',
             url +
                '?acao=salvarLancamento' +	
                '&unicod=<? echo $unicod  ?>' +
                '&nlcid=3'+
                '&cauid='+  tecbcd_cauid.value +
                '&carcod='+ tecbcd_carcod.value+ 
                '&calid='+  calid+
                '&lan2008='+tecbcd_2008.value+
                '&lan2009='+tecbcd_2009.value+
                '&lan2010='+tecbcd_2010.value+
                '&lan2011='+tecbcd_2011.value+
                '&lan2012='+tecbcd_2012.value,
                true        
        );

        limparCamposTecBCD();
        
        req.onreadystatechange = function(){ respostaAdicionarRegistroTecBCD(req); };
        req.send( null );        

	}
    

    function limparCamposTecBCD()
    {
        var form_campus	    = document.getElementById( 'tecbcd_cauid' );
		var form_2008		= document.getElementById( 'tecbcd_2008' );
		var form_2009		= document.getElementById( 'tecbcd_2009' );
		var form_2010		= document.getElementById( 'tecbcd_2010' );
		var form_2011		= document.getElementById( 'tecbcd_2011' );
		var form_2012		= document.getElementById( 'tecbcd_2012' );
        
        form_campus.setAttribute('calid', null);
        form_2008.value = '';
        form_2009.value = '';
        form_2010.value = '';
        form_2011.value = '';
        form_2012.value = '';
        
        corSimcorNao('tabela_tecbcd');
    }    
    

	function respostaAdicionarRegistroTecBCD(req)
	{
        if ( req.readyState != 4 )
        {
            mostraMensagemAguarde();
            return;
        }
			
        if ( req.status == 200 || req.status == 500  )
        {
            if(req.status == 500)
                alert("Observação:" + req.responseText);                
            
            carregaTelaTecBCD();
        }
        else
        {
            escondeMensagemAguarde();
            alert('erro:' + req.responseText);
        }
        
    }

	/**
	 * Prepara formulário para edição de um item da tabela financeiro.
	 *                                                           
	 */
	function editarRegistroTecBCD(linhaTabela, calid, campus, cargo, campo_2008,campo_2009,campo_2010,campo_2011,campo_2012)
	{	

		var form_campus	= document.getElementById( 'tecbcd_cauid' );
        var form_cargo	= document.getElementById( 'tecbcd_carcod' );
		var form_2008	= document.getElementById( 'tecbcd_2008' );
		var form_2009	= document.getElementById( 'tecbcd_2009' );
		var form_2010	= document.getElementById( 'tecbcd_2010' );
		var form_2011	= document.getElementById( 'tecbcd_2011' );
		var form_2012	= document.getElementById( 'tecbcd_2012' );

        form_2008.value = campo_2008;
		form_2009.value = campo_2009;
		form_2010.value = campo_2010;
		form_2011.value = campo_2011;
		form_2012.value = campo_2012;

        for (var i =0; i < form_campus.options.length; i++)
        {
            if (form_campus.options[i].getAttribute('value') == campus)
            {
                form_campus.options[i].selected = true;
                form_campus.setAttribute('calid', calid);
            }
        }
        

        for (var i =0; i < form_cargo.options.length; i++)
        {
            if (form_cargo.options[i].getAttribute('value') == cargo)
            {
                form_cargo.options[i].selected = true;
            }
        }   
        
         defineCorItemAlterado(linhaTabela, 'tabela_tecbcd');
        
	}    
    

	function carregaTelaTecBCD()
	{
        carregaListagemTecBCD(3,"tabela_tecbcd");
        carregaDisponivel(3,"tabela_tecbcd_disponivel");
        carregaLancado(3,"tabela_tecbcd_lancado");
	}
    carregaTelaTecBCD();
</script>

