
<?php

@session_start();

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/arquivo.inc';
include APPRAIZ . 'reuni/www/funcoes.php';
include APPRAIZ . 'reuni/www/_componentes.php';
print '<br/>';


$unidade = $db->usuarioUnidadesPermitidas('reuni');

if( ! in_array($_REQUEST['unicod'], $unidade))
{
	echo "<script>location.href = 'reuni.php?modulo=principal/listamonitoramento&acao=C' </script>";
}
$menu[0] = array("descricao" => "Lista de Unidades", "link"=> "/reuni/reuni.php?modulo=principal/listamonitoramento&acao=C");
$menu[1] = array("descricao" => "Monitoramento Acadêmico", "link"=> "/reuni/reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=".$_REQUEST['unicod']."");
$menu[2] = array("descricao" => "Formulário de Impressão", "link"=> "/reuni/reuni.php?modulo=principal/relatorio_monitoramento&acao=C&unicod=".$_REQUEST['unicod']."");

echo montarAbasArray($menu, "/reuni/reuni.php?modulo=principal/cadastromonitoramento&acao=C&unicod=".$_REQUEST['unicod']."");

//$db->cria_aba( $abacod_tela, $url, '' );
$db->cria_aba( $abacod_tela, $url, "&unicod=" . $_REQUEST['unicod'] );
monta_titulo( 'Monitoramento Acadêmico', '<img src="/imagens/atencao.png" align="middle"/> Itens com pendências
			&nbsp;&nbsp;&nbsp;
			<img src="/imagens/check_p.gif" align="middle"/> Itens ok' );

$_SESSION['unicod'] = $_REQUEST['unicod'];

//ajax para gravar na sessao todos os itens da arvore
if(isset($_REQUEST['evento']) &&  $_REQUEST['evento']== 'ajax_abrirarvore'){

	$sql_pais = "select
				distinct
				gp.grpcod as codpai
				from reuni.grupopergunta gp
				inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
				inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
				where p.prgord = 2";
				$dados_pais = $db->carregar($sql_pais);
				
				foreach ($dados_pais as $dado){
							$sql_filhos = "select
								gp2.grpcod as codfilho
								from reuni.grupopergunta gp
								inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
								inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
								where p.prgord = 2 and gp.grpcod = ".$dado['codpai']."";
							
							
							$dados_filhos = $db->carregar($sql_filhos);
							
							foreach($dados_filhos as $dado_filho){
								$indice = $dado['codpai']."_". $dado_filho['codfilho'];
								
								$_SESSION[ 'ArvoreSessaoReuni' ][$indice] = 1;
							}
							$_SESSION[ 'ArvoreSessaoReuniSrc' ][$dado['codpai']] = 'menos';
				}
		die('1');
}

//ajax para gravar na sessao todos os itens da arvore
if(isset($_REQUEST['evento']) &&  $_REQUEST['evento']== 'ajax_fechararvore'){

	$sql_pais = "select
				distinct
				gp.grpcod as codpai
				from reuni.grupopergunta gp
				inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
				inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
				where p.prgord = 2";
				$dados_pais = $db->carregar($sql_pais);
				
				foreach ($dados_pais as $dado){
							$sql_filhos = "select
								gp2.grpcod as codfilho
								from reuni.grupopergunta gp
								inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
								inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
								where p.prgord = 2 and gp.grpcod = ".$dado['codpai']."";
							
							
							$dados_filhos = $db->carregar($sql_filhos);
							
							foreach($dados_filhos as $dado_filho){
								$indice = $dado['codpai']."_". $dado_filho['codfilho'];
								
								$_SESSION[ 'ArvoreSessaoReuni' ][$indice] = 0;
							}
							$_SESSION[ 'ArvoreSessaoReuniSrc' ][$dado['codpai']] = 'mais';
				}
		die('1');
}

$sql = "
		select 
			codPai,
			dscPai,
			ordemPai,
			codFilho,
			dscFilho,
			ordemFilho,
			codPergunta
		from
			(
				select
					gp.grpcod as codPai,
					gp.grpdsc as dscPai,
					gp.grpord as ordemPai,
					gp2.grpcod as codFilho,
					gp2.grpdsc as dscFilho,
					gp2.grpord as ordemFilho,
					p.prgcod as codPergunta
					from reuni.grupopergunta gp
					inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
					inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
					where p.prgord = 2 
			) as foo
		order by
			ordemPai,
			ordemFilho
";

$result = $db->carregar($sql);


function procuraNoNaSessao( $strIdNo )
{
	if( isset( $_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] ) )
	{
		return $_SESSION[ 'ArvoreSessaoReuni' ][ $strIdNo ] ? "" : "none";
	}
	else
	{
		return "none";
	}
}

function procuraNoNaSessaoImagem( $strIdNo )
{
	if( isset( $_SESSION[ 'ArvoreSessaoReuniSrc' ][ $strIdNo ] ) )
	{
		return $_SESSION[ 'ArvoreSessaoReuniSrc' ][ $strIdNo ];
	}
	else
	{
		return "mais";
	}
}

?>
<!-- biblioteca javascript local -->
<script type="text/javascript"></script>
<!-- corpo da página -->
<script src="/includes/prototype.js"></script>
<script
	type="text/javascript" src="../../includes/JsLibrary/_start.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/tags/superTitle.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/_start.js"></script>
<style>
.TitleClass {
	background-color: #ffffcc;
	border: 1px solid #707070;
	color: #252525;
	font-size: 11px;
	font-weight: normal;
	padding: 3px 5px 3px 5px;
}
</style>
<table align="center" bgcolor="#f5f5f5" cellpadding="3" cellspacing="0"
	border="0" class="tabela" >
	<!-- ESQUERDA -->
	<tr>		
		<td>
		<table style="width: 100%;" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td align='right' class="SubTituloDireita">Unidade :</td>
				<td><?= nomeUnidade($_REQUEST['unicod'] ) ?></td>
			</tr>			
			<?php if ( reuni_podeVerResponsaveis( $_REQUEST['unicod'] ) ) : ?>			
			<tr>
				<td colspan="2" align='right' class="SubTituloEsquerda">Responsabilidades</td>
			</tr>
			<?= $db->mostra_resp( $_REQUEST['unicod'], 'unicod', false, 'reuni' ); ?>
			<?php endif; ?>
		</table>
		</td>
	</tr>			
	<tr>
		<td>
			<b>As dimensões do plano de	reestruturação</b>
			<p>
				<a href="javascript: abrirTodos();">Abrir Todos</a>
				 | 
				<a href="javascript: fecharTodos();">Fechar Todos</a>
			</p>
		</td>
	</tr>		
	<tr>
		<td width="100%">
		<form action="" method="post" name="formulario">
		<table id="arvore" class="tabela" style="width: 100%">			
			<tr style="background-color: #e0e0e0" height="25px">		
				<td style="font-weight:bold; text-align:center;" width="45%">Título</td>
				<td style="font-weight:bold; text-align:center;" width="15%">Situação</td>
				<td style="font-weight:bold; text-align:center;" width="15%">Qualificação</td> 
				<td style="font-weight:bold; text-align:center;" width="25%">Última Atualização</td> 
			</tr>		
			<?
			$i = 0;
			while ($i < count($result))
			{
				?>
				<tr style="cursor: pointer" class="nivel1"
					id="<?= $result[$i]['codpai']?>" parent="">
					<td  style="padding-left: <?php if($result[$i]['codpai'] >= 28) echo ''; else echo'20px'?>" onclick="FechaElemento( '<?= $result[$i]['codpai']?>' )">
					<img
						src="/imagens/<?=procuraNoNaSessaoImagem(  $result[$i]['codpai'] )?>.gif" />
					<b> <?php 					
					echo $result[$i]['dscpai'];	
					?> </b></td>
				</tr>
				<?php
				$codPai     = $result[$i]['codpai'];
				$codPaiNovo = $result[$i]['codpai'];
	
				$t=1;
				while($codPai == $codPaiNovo )
				{
					$codFilho = $result[$i]['codfilho'];
					$codFilhoNovo = $codFilho;
						
					if($codFilho != '')
					{
						
						$sql_preenchido ="select max(to_char(monit.mondata, 'DD-MM-YYYY HH24:MI:SS')) as data, usu.usunome as nome  from reuni.monitoramento monit
						left join seguranca.usuario usu on monit.usucpf = usu.usucpf
						where monit.unicod = '".$_REQUEST['unicod']."' and monit.grpcod = ".$codFilho."
						group by usu.usunome
						";
												
						$preenchido =  $db->pegaLinha($sql_preenchido);
						
						$sql_dados = "
							select 
							  to_char (monit.monpercentual, '999%')  as andamento 
							, trim (sta.stadsc) as status
							, sta.staid
							, sit.sitdsc as situacao
							from reuni.monitoramento monit
							left join reuni.status sta on sta.staid = monit.staid	
							left join reuni.situacao sit on sit.sitcod = monit.sitcod						
							where monit.unicod = '".$_REQUEST['unicod']."' and monit.grpcod = ".$codFilho."
						";
						
						$dados = $db->pegaLinha($sql_dados);
						
						$status = $dados['status'] ? $dados['status'] : "Não Informado";
						$situacao = $dados['situacao'] ? $dados['situacao'] : "Não Informado";
						$andamento = $dados['andamento'] ? $dados['andamento'] : "Não Informado";
						$atividade = array('staid' => $dados['staid'], 'stadsc' => $status, 'andamento' => $andamento);
						
						$imagem_preenchimento = "";
						$title = "";
						if( ! $preenchido){
							$imagem_preenchimento = "<img src='/imagens/atencao.png' align='top'/>";
							$title = "";
							
						}else{
							$imagem_preenchimento = "<img src='/imagens/check_p.gif' align='top'/>";
							//$title = "onmouseover=\"return escape('".$preenchido['nome']." - ".$preenchido['data']."');\"";							
						}
						
						$a =  $title.' href="/'.$_SESSION['sisdiretorio'].'/'.$_SESSION['sisarquivo'].'.php?modulo=principal/propostamonitoramento&acao=C&id='.md5_encrypt($result[$i]['codpergunta'] .'_'.$result[$i]['codpai'] .'_'.$result[$i]['codfilho'] ,'').'&unicod='.base64_encode($_REQUEST['unicod']).'&len='.base64_encode($result[$i]['qtdcaracteres']).'"';
						
						$atualizacao = "";
						if( $preenchido['nome'] && $preenchido['data']){
							$atualizacao = $preenchido['nome']." em  ".$preenchido['data'];
						}
						
						
						if($t % 2){
							$bg = "bgcolor='#FAFAFA'";
						}else{
							$bg = "bgcolor='#F0F0F0'";
						}
						
						?>
				<tr <?=$bg ?> style="display:<?=procuraNoNaSessao( $result[$i]['codpai'] . '_' . $codFilho )?>" class="nivel2" id="<?= $result[$i]['codpai'] . '_' . $codFilho ?>" parent="<?= $result[$i]['codpai']?>"  onclick="FechaElemento( '<?=  $result[$i]['codpai'] . '_' . $codFilho ?>')">
					<td style="padding-left: 30px">&nbsp;&nbsp;&nbsp;&nbsp;<?=$imagem_preenchimento; ?>
					 <?php/* echo $result[$i]['dscfilho']; echo filhoPreenchido($_REQUEST['unicod'],$result[$i]['codfilho'])	*/?>				
					 <a <?=$a ?>><?=$result[$i]['dscfilho'] ?></a>			
					</td>
					<td align="center" >
					<?= montar_barra_execucao( $atividade, true ) ?>
					</td>						
					<td style="text-align:center;" width="15%"><?=$situacao ?></td>
					
					<td style="text-align:center;" width="15%"><?=$atualizacao ?></td>
				</tr>
				<?
	
			$i++;
			$codPaiNovo = $result[$i]['codpai'];
			}
			$t++;
		}
	
	}
		?>			
		</table>
		</form>
		</td>
	</tr>
</table>


<script type="text/javascript">
	
	
	
	var dav = navigator.appVersion ;
	IE = document .all ? true : false;
	
	
	function reloadPage(){
		window.location.reload();
	}		
	
	function versao_impressao()
	{		
		window.open('reuni.php?modulo=principal/versaoimpressao&acao=C&unicod=<?=base64_encode($_REQUEST['unicod'])?>','blank','width=800,height=600,status=0,toolbar=0,location=0,scrollbars=1,resizable=1');	
	}	
	
	function FechaElemento( strIdElemento , _boolFecha )
	{
		var objElemento = document.getElementById( strIdElemento );
		var arrAlterados = Array();
		var boolPrimeiraChamada = false;
		if( _boolFecha == undefined )
		{
			boolPrimeiraChamada = true;
			var objImg = objElemento.getElementsByTagName( "img" )[0];
			var strSrc = objImg.src;
			_boolFecha = ( strSrc.indexOf( "menos" ) > 0 );
			var strPedaco;
			if( _boolFecha )
			{
				strSrc = strSrc.replace( "menos" , "mais" );
				strPedaco = "mais";
	
			}
			else
			{
				strSrc = strSrc.replace( "mais" , "menos" );
				strPedaco = "menos";
			}
			objImg.src = strSrc;
			arrParams = Array( objElemento.id , strPedaco );
			addRequest( "../atualizaArvoreSessaoReuni.php" , "mudaImg" , arrParams, function(){}  );
	
		}
		arrTrElementos = ( objElemento.parentNode.getElementsByTagName( "tr" ) );
		var strEvento = _boolFecha ? "fechaNo" : "mostraNo";
		var arrParams = Array();
	
		for( var i = 0; i < arrTrElementos.length; i++ )
		{
			var objTrElemento = arrTrElementos[ i ];
			if( objTrElemento.getAttribute( "parent" ) == objElemento.id )
			{
				if( _boolFecha )
				{
					arrParamsFilhos = FechaElemento( objTrElemento.id , _boolFecha );
					arrParams = arrParams.concat( arrParamsFilhos );
				}
				else
				{
					if( objTrElemento.getElementsByTagName( "img").length > 0 )
					{
						var objImg = objTrElemento.getElementsByTagName( "img" )[0];
						var strSrc = objImg.src;
						strSrc = strSrc.replace( "menos" , "mais" );
						objImg.src = strSrc;
					}
				}
				var parametro;
				if(IE)
				parametro =  "block";
				else
				parametro = "table-row";
	
				objTrElemento.style.display = _boolFecha ? "none" : parametro;
	
				arrParams.push( objTrElemento.id );
			}
		}
	
		if( boolPrimeiraChamada )
		{
			addRequest( "../atualizaArvoreSessaoReuni.php" , strEvento , arrParams, function(){}  );
		}
		return arrParams;
	}
	
	
	
	function fecharTodos()
	{
		var arvore = document.getElementById( 'arvore' );		
		var linhas =  arvore.getElementsByTagName("tr");	
		var imagens = arvore.getElementsByTagName("img");
			
		for(i = 0; i < linhas.length; i++ ){
			if(linhas[i].id.indexOf('_') > -1){
				linhas[i].style.display = 'none';                      
			}
		}
				
		for(i = 0; i < imagens.length; i++ ){			
			if(imagens[i].src.indexOf('menos') > -1){	
				imagens[i].src = imagens[i].src.replace( "menos" , "mais" );
			}
		}	

		ajax_enviar('ajax_fechararvore');
				
	}
	
	function abrirTodos()
	{
		var arvore = document.getElementById( 'arvore' );		
		var linhas =  arvore.getElementsByTagName("tr");	
		var imagens = arvore.getElementsByTagName("img");	
		
		for(i = 0; i < linhas.length; i++ ){
			if(linhas[i].id.indexOf('_')){							
				linhas[i].style.display = document.all ? 'block' : 'table-row';				                      
			}
		}	
		
		for(i = 0; i < imagens.length; i++ ){			
			if(imagens[i].src.indexOf('mais') > -1){	
				imagens[i].src = imagens[i].src.replace( "mais" , "menos" );
			}
		}

		ajax_enviar('ajax_abrirarvore');
				
	}

	function ajax_enviar(acao){
		
		var url = location.href;
		var parametros = "&evento=" + acao;
		
		var myAjax = new Ajax.Request(
			url,
			{
				method: 'post',
				parameters: parametros,
				asynchronous: true,
				onComplete: function(resp) {						
				}
			}
		);	
	}
	
	

</script>
