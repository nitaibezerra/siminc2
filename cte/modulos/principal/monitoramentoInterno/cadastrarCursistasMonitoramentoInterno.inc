<?php

header('Cache-control: must-revalidate');
header('Expires: Fri, 01 Jan 1999 00:00:00 GMT');

if( !$_REQUEST['mniano'] || !$_REQUEST['inuid'] || !$_REQUEST['ppsid'] ){ die();}

include_once( APPRAIZ . "cte/classes/MonitoramentoInterno.class.inc" );
include_once( APPRAIZ . "cte/classes/MonitoramentoInternoEntidade.class.inc" );

if( $_REQUEST['req'] == 'removerCursista' ){

	if( $_REQUEST['mneid'] ){
		
		$obMonitoramentoInternoEntidade = new MonitoramentoInternoEntidade();
		$obMonitoramentoInternoEntidade->excluir( $_REQUEST['mneid'] );
		$obMonitoramentoInternoEntidade->commit();
	}
	die();
	
}
include_once( APPRAIZ . "cte/classes/SituacaoCursista.class.inc" );
include_once( APPRAIZ . 'includes/classes/entidades.class.inc');
include_once( APPRAIZ . "www/includes/webservice/cpf.php" );

$obMonitoramentoInterno = new MonitoramentoInterno();
$obMonitoramentoInterno->carregarPorInuidPpsidAno( $_REQUEST['inuid'], $_REQUEST['ppsid'], $_REQUEST['mniano'] );

$obSituacaoCursista = new SituacaoCursista();
$arSituacaoCursista = $obSituacaoCursista->recuperarTodos( 'scuid as codigo, scudsc as descricao' );
if( array_key_exists('btnGravar', $_REQUEST) ){

	$_REQUEST["mneid"] = ($_REQUEST["mneid"]) ? $_REQUEST["mneid"] : array();
	
	$mniqtd = is_array( $_REQUEST['mneid'] ) ? count( $_REQUEST['mneid'] ) : '0';	
	
	if( verificaPerfisAdiminMonitoramentoInterno() ) {
			
		$obMonitoramentoInterno->usucpfvalida = $_SESSION['usucpf'];
		$obMonitoramentoInterno->mnidtvalidacao = date('Y-m-d');
									
	} else {
		
		$obMonitoramentoInterno->usucpf = $_SESSION['usucpf'];
	}
		
	$obMonitoramentoInterno->mniqtd = $mniqtd;
	$obMonitoramentoInterno->salvar();
	$obMonitoramentoInterno->commit();
	
	foreach( $_REQUEST["mneid"] as $count => $mneid ){
		
		if(!$_POST["cpf_$count"]){
			$db->rollback();
			echo "<script>
		        	alert('O campo CPF é obrigatório.');
					history.back(-1);
				  </script>";
			die;
		}
		
		if(!$_POST["nome_$count"]){
			$db->rollback();
			echo "<script>
		        	alert('O campo Nome é obrigatório.');
					history.back(-1);
				  </script>";
			die;
		}
		
		
		$cpf = ereg_replace("[^0-9]", "", $_POST["cpf_$count"] );
		$sql = "select entid from entidade.entidade where entnumcpfcnpj = '".$cpf."'";
		$entid = $db->pegaUm($sql);					
		
		if( !$entid ){ # se não tem cpf na base, cadastra
			$arDados = array( 'entnumcpfcnpj' => $cpf, 'entnome' => $_POST["nome_$count"] );
			$obEntidade = new Entidades();
			$obEntidade->carregarEntidade($arDados);
			$obEntidade->salvar();
			$entid = $obEntidade->getEntid();
		}
		
		$obMonitoramentoInternoEntidade = new MonitoramentoInternoEntidade( $mneid );
		$obMonitoramentoInternoEntidade->entid = $entid;
		$obMonitoramentoInternoEntidade->scuid = $_POST["scuid_$count"];
		$obMonitoramentoInternoEntidade->mniid = $obMonitoramentoInterno->mniid;
		
		$obMonitoramentoInternoEntidade->salvar();
		$obMonitoramentoInternoEntidade->commit();
	
			
	}		
	
    echo "	<script>
			    alert('Operação Efetuada com sucesso!');
			    window.opener.location = 'cte.php?modulo=principal/monitoramentoInterno/cadastrarMonitoramentoInterno&acao=A&ppsid=".$obMonitoramentoInterno->ppsid."&mniano=".$obMonitoramentoInterno->mniano."'
    		</script>";
    exit();		
	
}

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
	<body>
		<div id="loader-container">
	      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
	    </div>
		<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="frmCursistas" onsubmit="return validarfrmCursistas();">
			<input type="hidden" name="" />
			<table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
				<tr style="background-color: #cccccc;">
					<td>
						<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Dados dos Cursistas</div>
						<div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
		            		<table id="itensCursistas" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
		              			<colgroup>
					                <col width="2%" />
					                <col width="10%" />
					                <col width="10%" />
					                <col width="10%" />
								</colgroup>
		              			<tr style="text-align: center">
									<td style="padding: 2px; font-weight: bold;">&nbsp;</td>
									<td style="padding: 2px; font-weight: bold;">CPF<input type="hidden" name="boCPF" value="1" /></td>
									<td style="padding: 2px; font-weight: bold;">Nome</td>
									<td style="padding: 2px; font-weight: bold;">Situação</td>
								</tr>
		            		</table>
	          			</div>
	          			<div style="padding: 5px 0px 0px 5px">
	    					<span style="cursor:pointer" id="linkInserirItem" onclick="return carregarItensCPF( '', '', '', '', '', '' );"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir dados dos cursistas</span>
	          			</div><br />
					</td>
				</tr>
				<tr style="background-color: #cccccc;">
					<td class="SubTituloDireita">
						<span style="display: block;font-weight:bold;font-size:110%;margin-bottom:5px;color:red">Atenção! Itens sem identifição serão descartados</span>
						<input type="submit" name="btnGravar" value="Gravar" />
						<!-- <input type="button" name="btnFechar" value="Fechar" onclick="fecharJanela();" />  -->
					</td>
				</tr>
			</table>
		</form>
	</body>

	<script type="text/javascript">
	    
	    function selecionarTodas()
	    {
	        var items = Form.getElements($('frmCursistas'));
	
	        for (var i = 0; i < items.length; i++) {
	            if (items[i].getAttribute('type') == 'checkbox' &&
	                /entid/.test(items[i].getAttribute('id')))
	            {
	                items[i].checked = $('selecionar').checked;
	            }
	        }
	    }
	
	    function validarfrmCursistas()
	    {
	        var checks = $('frmCursistas').getInputs('checkbox', 'entid[]');
	        var submit = true;
	        
	        for (var i = 0, length = checks.length; i < length; i++) {
	            if (checks[i].checked || checks[i].checked == 'checked') {
	                var qfaqtd = $('frmCursistas').getInputs('text');
					
					if(qfaqtd[i+1].value == '' || qfaqtd[i+1].value == 0 ){
						 //qfaqtd.setStyle({backgroundColor: '#ffd'});
						 submit = false;
					}
					
	            }
	        }
	        
	        var tabela = document.getElementById('itensCursistas');
	        var nrLinhas = tabela.rows.length;
	        
	        for(i=1; i<nrLinhas; i++){
	        
	        	var nmLinha1 = tabela.rows[i].id.substr(6);;	        
	        	//alert(nmLinha1);
	        	
	        	for(x=1; x<i; x++){
	        	
	        		var nmLinha2 = tabela.rows[x].id.substr(6);	        		
	        		var cpf1 = replaceAll(replaceAll(document.getElementById('cpf_'+nmLinha1).value, ".", ""), "-", "");
	        		var cpf2 = replaceAll(replaceAll(document.getElementById('cpf_'+nmLinha2).value, ".", ""), "-", "");
	        		
	        		//alert(cpf1+" = "+cpf2);
	        		
	        		if(cpf1 == cpf2){
	        			alert("Existem cpfs duplicados!");
	        			document.getElementById('cpf_'+nmLinha1).focus();
	        			return false;
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
	
		function removerLinha( idLinha, mneid ){
			
			if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
				return false;
			}
		
			new Ajax.Request(window.location.href,{
				method: 'post',
				parameters: '&req=removerCursista&mneid=' + mneid
			});
		                               	
			var tabela 	= document.getElementById( "itensCursistas" );
			var linha 	= document.getElementById( idLinha );
		
			tabela.deleteRow( linha.rowIndex );
		
		}
		
		function procurarNome( cpf, qtd ){
			var data = '';
			var comp = new dCPF();
			var arNome = document.getElementsByName( 'nome_' + qtd );
			
			comp.buscarDados( cpf );
			if (comp.dados.no_pessoa_rf != ''){
				arNome[0].value = comp.dados.no_pessoa_rf;
				arNome[0].readOnly = true;
			}
		}
		
		function eventosIE( obj, tipo ){
			
			switch( tipo ){
				case('1'): MouseBlur(obj); validarcpf( obj ); break;
				case('2'): MouseOut(obj); break;
				case('3'): MouseClick(obj); obj.select(); break;
				case('4'): MouseOver(obj); break;
				case('5'): obj.value=mascaraglobal('###.###.###-##',obj.value); break;
			}
			
		}
		
		function eventosIECPF( obj, qtd ){
			procurarNome( obj.value, qtd );
		}
		
		function validarcpf( obj ){
			
			if( obj.value ){
				if( !validar_cpf( obj.value  ) ){
					alert( "CPF inválido!\nFavor informar um cpf válido!" );
					obj.value = "";	
				}
			}
		}
		
		function carregarItensCPF( mneid, entcpf, entnome, scuid ){
		
			var tabela 	= document.getElementById( "itensCursistas" );
			var qtdLinha = tabela.rows.length;
			
			var qtd = qtdLinha - 1;
													
			linha = tabela.insertRow( tabela.rows.length );

			linha.setAttribute( "id", "linha_" + qtd );
			linha.setAttribute( "style", "background: #f5f5f5;" );
			linha.setAttribute( "align", "center" );
													
			var img = document.createElement( "img" );
			img.setAttribute( "src", "/imagens/excluir.gif" );
			img.setAttribute( "title", "Excluir" );
			img.setAttribute( "alt", "Excluir" );
		
			// OnBlur
			if(window.addEventListener){ // Mozilla, Netscape, Firefox
				img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "', '"+ mneid +"' )" );
			}
			else{ // IE
				img.attachEvent( "onclick", function() { removerLinha( "linha_" + qtd , mneid ) } );
			}
								
			var cpf = document.createElement( "input" );
			cpf.setAttribute( "type", "text" );
			cpf.setAttribute( "name", "cpf_" + qtd );
			cpf.setAttribute( "id", "cpf_" + qtd );
			cpf.setAttribute( "style", "width: 21ex;" );
			cpf.setAttribute( "size", "19" );
			cpfValorFormatado = "";
			if(cpf){ cpfValorFormatado = mascaraglobal('###.###.###-##', entcpf ); }
			cpf.setAttribute( "value", cpfValorFormatado );
			cpf.className = "normal";
			cpf.setAttribute('onkeyup', "this.value=mascaraglobal('###.###.###-##',this.value);");
		
			// OnBlur
			if(window.addEventListener){ // Mozilla, Netscape, Firefox
				cpf.setAttribute( "onblur", "MouseBlur(this); validarcpf( this );" );
				cpf.setAttribute( "onmouseout", "MouseOut(this);" );
				cpf.setAttribute( "onfocus", "MouseClick(this); this.select();" );
				cpf.setAttribute( "onmouseover", "MouseOver(this);" );
				cpf.setAttribute( "onkeyup", "this.value=mascaraglobal('###.###.###-##',this.value);" );
				cpf.setAttribute( "onchange", "procurarNome( this.value, "+ qtd +" )" );
			}
			else{ // IE
				cpf.attachEvent( "onblur", 		function() { eventosIE( document.getElementById( 'cpf_' + qtd ), "1" ); } );
				cpf.attachEvent( "onmouseout", 	function() { eventosIE( document.getElementById( 'cpf_' + qtd ), "2" ); } );
				cpf.attachEvent( "onfocus", 	function() { eventosIE( document.getElementById( 'cpf_' + qtd ), "3" ); } );
				cpf.attachEvent( "onmouseover", function() { eventosIE( document.getElementById( 'cpf_' + qtd ), "4" ); } );
				cpf.attachEvent( "onkeyup", 	function() { eventosIE( document.getElementById( 'cpf_' + qtd ), "5" ); } );
				//cpf.attachEvent( "onkeyup", 	function() { eventosIECPF( document.getElementById( 'cpf_' + qtd ), qtd ); } );
			}
		
			var nome = document.createElement( "input" );
			nome.setAttribute( "type", "text" );
			nome.setAttribute( "name", "nome_" + qtd );
			nome.setAttribute( "id", "nome_" + qtd );
			nome.setAttribute( "style", "width: 33ex;" );
			nome.setAttribute( "size", "30" );
			nome.setAttribute( "value", entnome );
			nome.className = "disabled";
			
			var entid = document.createElement( "input" );
			entid.setAttribute( "type", "hidden" );
			entid.setAttribute( "name", "entid_" + qtd );
			entid.setAttribute( "value", qtd );
			
			var input_mneid = document.createElement( 'input' );
			input_mneid.setAttribute( 'type', 'hidden' );
			input_mneid.setAttribute( 'name', 'mneid[]' );
			input_mneid.setAttribute( 'value', mneid );
                     
			celula = linha.insertCell( 0 );
			celula.appendChild( img );
			celula.appendChild( input_mneid );
			
			var imgObrigatorio = document.createElement( "img" );
			imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
			imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );
		
			var imgObrigatorio2 = document.createElement( "img" );
			imgObrigatorio2.setAttribute( "src", "../imagens/obrig.gif" );
			imgObrigatorio2.setAttribute( "title", "Indica campo obrigatório." );
		
			var imgObrigatorio3 = document.createElement( "img" );
			imgObrigatorio3.setAttribute( "src", "../imagens/obrig.gif" );
			imgObrigatorio3.setAttribute( "title", "Indica campo obrigatório." );												
			
			celula = linha.insertCell( 1 );
			celula.appendChild( cpf );
			celula.appendChild( imgObrigatorio );
			
			celula = linha.insertCell( 2 );
			celula.appendChild( nome );

			var comboSituacaoCursista = document.createElement( 'select' );
			comboSituacaoCursista.setAttribute( 'name', "scuid_" + qtd );
			
			var option = document.createElement( 'option' );
			option.setAttribute( 'value', '' );
			option.innerHTML = 'Selecione';
			comboSituacaoCursista.appendChild( option );

			<?php 
			$stCursistas = '';
			foreach( $arSituacaoCursista as $arDadosSituacaoCursista ){ ?>

				var option = document.createElement( 'option' );
				option.setAttribute( 'value', '<?php echo $arDadosSituacaoCursista["codigo"]; ?>' );

				if( scuid == '<?php echo $arDadosSituacaoCursista["codigo"]; ?>' ){
					option.setAttribute( 'selected', 'selected' );
				}
					
				option.innerHTML = '<?php echo $arDadosSituacaoCursista["descricao"]; ?>';
				comboSituacaoCursista.appendChild( option );
			<?php } ?>

			celula = linha.insertCell( 3 );
			celula.appendChild( comboSituacaoCursista );
			
		}
	
		<?php 
			$sql = "select distinct  mneid, e.entnumcpfcnpj, e.entnome, scu.scudsc, scu.scuid
								from cte.monitoramentointerno mi
									inner join cte.monitoramentointernoentidade mie on mie.mniid = mi.mniid
									inner join entidade.entidade e on e.entid = mie.entid
									left  join cte.situacaocursista scu on scu.scuid = mie.scuid 
								where mi.inuid = '{$_REQUEST['inuid']}'
								and mi.ppsid = '{$_REQUEST['ppsid']}'
								and mi.mniano = '{$_REQUEST['mniano']}'
								order by e.entnome";
			
			$arMonitoramentoInternoEntidade = $db->carregar( $sql );
			$arMonitoramentoInternoEntidade = $arMonitoramentoInternoEntidade ? $arMonitoramentoInternoEntidade : array();
			
			foreach( $arMonitoramentoInternoEntidade as $arLinha ){
				echo "carregarItensCPF( '{$arLinha['mneid']}', '{$arLinha['entnumcpfcnpj']}', '{$arLinha['entnome']}', '{$arLinha['scuid']}' );";	
			}
		?>
			
	    $('loader-container').hide();
	</script>
	
	
</html>
