<?php

include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');


if($_REQUEST['prsid'] && !$_REQUEST['salvar']){
	
	$sqlSubacoes = "select distinct
					'<input type=\"checkbox\" name=\"subacoes[]\" id=\"subacoes[]\" value=\"'|| si.sbaid ||'\"'
						|| 
						CASE WHEN  apf.sbaid is not null AND apf.apftipo = '{$_REQUEST['tipo']}'
						THEN 'checked' 
						ELSE '' END ||
						'>' as acao,
					si.sbaordem || ' - ' || si.sbadsc as descricao,
					si.sbaporescola as sbaporescola,
					si.sbaid as sbaid					 
				from cte.projetosapesubacao ps
				inner join cte.subacaoindicador si on si.sbaid = ps.sbaid
				inner join cte.projetosape pr on pr.prsid = ps.prsid
				inner join cte.instrumentounidade iu on iu.inuid = pr.inuid
				left join parindigena.associativaplanodemetasformacao apf on apf.sbaid = si.sbaid
				where pr.prsid = {$_REQUEST['prsid']}
				and iu.estuf = '{$_REQUEST['estuf']}'";
	$dados = $db->carregar($sqlSubacoes);
	
	
	$sql = "SELECT									
				covnumero
			FROM parindigena.convenioindigena AS ci
			INNER JOIN financeiro.convenio AS cf ON ci.covcod = cf.covcod
			INNER JOIN cte.projetosape AS ps ON cf.covprocesso::numeric = ps.prsnumeroprocesso
			WHERE ps.prsid = {$_REQUEST['prsid']}";
	$rs = $db->carregar( $sql );
	$convenioAno = $rs[0]['covnumero'];
	$dadosConvenio = explode('/', $convenioAno);
	$numConvenio = $dadosConvenio[0];
	$ano = $dadosConvenio[1]; 
	
	$i = 0;
	
	foreach($dados as $subacoes){
		$dadosValor        = calculoValorProgramado($subacoes['sbaid'], $ano, $subacoes['sbaporescola']);
		$dados[$i]['valor'] = "R$ ".number_format(str_replace(',','',$dadosValor['cronograma']),2,',','.');
		unset($dados[$i]['sbaporescola']);
		unset($dados[$i]['sbaid']);
		$i++;
	}
	
	$cabecalho = array( "Ação", "Descrição da Subação", "Valor" );


	$obMontaListaAjax = new MontaListaAjax($db, false);
	echo '<form name="formulario" id="formulario" action="" method="post">';
	echo '	<input type=hidden name="salvar" value="false" />
			<input type=hidden name="todassub" id="todassub" value="" />';
	echo '	<center>
				<div class="SubTituloDireita" style="text-align:center;width:95%;">
					Selecione as subações referentes ao Programa de Formação Diferenciado, Intercultural e Bilíngue de Professores Indígenas
				</div>
			</center>';
	$obMontaListaAjax->montaLista($dados, $cabecalho,200, 10, 'S', '', '', '', '', '', '', '' );
	echo '<div style="text-align:center;"><input type="button" name="salvar" value="Vincular subações"  onclick="salvarDados();" /></div>';
	echo '</form>';
	die();
}

if($_REQUEST['salvar']){
	$totalSub 	= count($_REQUEST['subacoes']);
	$todasSub 	= explode(',', $_REQUEST['todassub']);
	
	
	if($totalSub > 0 && $_REQUEST['prsid'] ){
		
		$executado = false;
		$sql = "SELECT sbaid FROM parindigena.associativaplanodemetasformacao  WHERE  sbaid IN ('".implode("', '",$_REQUEST['subacoes'])."') AND prsid = ".$_REQUEST['prsid'];			
		$subExisteBanco = $db->carregar($sql);
		if(is_array($subExisteBanco)){
			foreach($subExisteBanco as $sub){
				if (in_array($sub['sbaid'], $todasSub)) { 
					$sql = "DELETE FROM parindigena.associativaplanodemetasformacao  WHERE  prsid = ".$_REQUEST['prsid']." and sbaid = ".$sub['sbaid'];
					$db->executar($sql);
				}
			}
		}
		
		foreach($_REQUEST['subacoes'] as $subacao){
			$sql = "INSERT INTO parindigena.associativaplanodemetasformacao (sbaid, apftipo, prsid) VALUES (".$subacao.", ".$_REQUEST['tipo'].", ".$_REQUEST['prsid'].")";
			if($db->executar($sql)){
				$executado = true;
			}
		}
		if($executado){
			$db->commit();
			echo "<script> 
					alert( 'Associação feita com sucesso. ' ); 
			  	  </script>";
		}else{
			$db->rollback();
			echo "<script> 
					alert( 'Ocorreu um erro ao tentar salvar a vinculação entre os dados. ' ); 
					window.close();
			  	  </script>";
			die();
		}
	}
}

?>
<html>
	<head>
	
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="-1">
	    <title><?php echo $titulo ?></title>
	
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script src="../includes/prototype.js"></script>
	    <script src="../includes/entidades.js"></script>
	    <script type="text/javascript" src="../includes/JQuery/jquery.js"></script> 
	    <script type="text/javascript"> jQuery.noConflict(); </script>  
	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    
	
		<script type="text/javascript">		
		
			function salvarDados(){
			
				var arIdSbaid 	 = document.getElementsByName('subacoes[]');				
				var TotalIdSbaid = arIdSbaid.length;
				var sucesso = false;
				var todasSubs = new Array();
				if(TotalIdSbaid > 0){
					for( x=0; x<TotalIdSbaid; x++ ){
						todasSubs[x] = arIdSbaid[x].value;
						if(arIdSbaid[x].checked){
							sucesso = true;
						}
					}
					$('todassub').value = todasSubs;
				}
				
				if(sucesso){
//				alert(arIdSbaid.length+" - "+todasSubs.length);
//				return false;
					document.formulario.salvar.value = true;
					document.formulario.submit();
				}else{
					alert('Selecione pelo menos uma subação.');
					return false;
				}
			}
	
			function carregarSubacoes(prsid){
			
				var conteudoSubacoes = $('mostraSubacao');
				
				new Ajax.Request('parindigena.php?modulo=principal/vincularConvenioSubacaoTipo&acao=A&tipo=<?=$_REQUEST['tipo'];?>&estuf=<?=$_REQUEST['estuf'];?>',
				{
					method: 'post',
					parameters: '&mostraLista=true&prsid='+prsid,
					onComplete: function(res){
						
						conteudoSubacoes.innerHTML = res.responseText;
						extraiScript(res.responseText);
					}
				});
			
			}
			
			function extraiScript(texto){  
			
				//desenvolvido por Skywalker.to, Micox e Pita.  
				//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
				var ini, pos_src, fim, codigo;  
				var objScript = null;  
				ini = texto.indexOf('<script', 0)  
				while (ini!=-1){  
				var objScript = document.createElement("script");  
				//Busca se tem algum src a partir do inicio do script  
				pos_src = texto.indexOf(' src', ini)  
				ini = texto.indexOf('>', ini) + 1;
		
				//Verifica se este e um bloco de script ou include para um arquivo de scripts  
				if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
					//Marca como sendo o inicio do nome do arquivo para depois do src  
					ini = pos_src + 4;  
					//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
					fim = texto.indexOf('.', ini)+4;  
					//Pega o nome do arquivo  
					codigo = texto.substring(ini,fim);  
					//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
					codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
					// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
					objScript.src = codigo;  
				}else{
				//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
					// Procura o final do script
					fim = texto.indexOf('</script', ini);  
					// Extrai apenas o script
					codigo = texto.substring(ini,fim);  
					// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
					objScript.text = codigo;
				}
				
				//Adiciona o script ao documento  
				document.body.appendChild(objScript);
				
				// Procura a proxima tag de <script>  
				ini = texto.indexOf('<script', fim);
				
				//Limpa o objeto de script  
				objScript = null;  
			}  
		} 
			
		</script>
	
	</head>
	
	<body marginheight="0" marginwidth="0">
			
    	<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
    	 	<tr style="background-color: #cccccc;">
        		<td colspan="2">
        			<div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Vinculação de subações entre Par Indígena e Par Genérico</div>
        		</td>
        	</tr>
        	<tr>
        		<td colspan="2" class="SubTituloEsquerda">
        		Filtros para lista de subação:
        		</td>
        	</tr>
        	<tr>
        		<td class="SubtituloEsquerda" >
        		Convênio:
        		</td>
        		<td>
        			<?php
					$sql = "SELECT									
								cf.estuf,
								ps.prsid as codigo,
								cf.covnumero as descricao,
								cf.covvalor, 
								ci.covcod
							FROM parindigena.convenioindigena AS ci
							INNER JOIN financeiro.convenio AS cf ON ci.covcod = cf.covcod
							INNER JOIN cte.projetosape AS ps ON cf.covprocesso::numeric = ps.prsnumeroprocesso
							INNER JOIN cte.instrumentounidade AS iu ON iu.inuid = ps.inuid 
							WHERE ci.coistatus = 'A'
							and cf.estuf = '{$_REQUEST['estuf']}' and iu.estuf = '{$_REQUEST['estuf']}'
							and ps.prsnumconvsape is not null
							 ";	

					$temConvenio = $db->pegaLinha($sql);
					if(!$temConvenio){
						echo "<span style=\"color:red;\" >Não existe convênio cadastrado no módulo 'PAR - Plano de Metas' ou no módulo Orçamento e Finanças'.</br> Verifique com os gestores dos sistemas.</span>";
					}else{
        				echo $db->monta_combo('convid',$sql,'S','Selecione um convênio...','carregarSubacoes','','','','S','convid','','',''); 
					}
        			?>
        		</td>
        	</tr>        		
        	<tr>
        		<td colspan="2" class="SubTituloDireita" style="height:20px;"></td>
        	</tr>
        </table>
        <div id="mostraSubacao"></div>
    	
    </body>
</html>