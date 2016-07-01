<?php 

function remove_acentos($str){
	$str = trim($str);
	$str = strtr($str,"¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ!@#%&*()[]{}+=?",
			"YuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy_______________");
	$str = str_replace("..","",str_replace("/","",str_replace("\\","",str_replace("\$","",$str))));
	return str_replace(array(' ',':'),array('_',''),strtolower($str));
}

$sql = "select * from maismedicos.ws_respostas_formulario where idpublicarformulario = '{$_GET['id']}'";
$rsForm = $db->pegaLinha($sql);

$sql = "select * from maismedicos.ws_respostas_formulario_itens where idpublicarformulario = '{$_GET['id']}'";
$rsFormItens = $db->carregar($sql);

?>

<html>

	<head>
	
		<title>Relatório de Distribuição Médicos Visitados por Supervisor - Mais Médicos</title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>		
		
	</head>
	
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
		<div style="padding:15px;">
			
				<?php 
				if($rsFormItens){
					$txt  = '<center><img src="http://simec.mec.gov.br/imagens/brasao.gif" height="65" /><h2>MINISTÉRIO DA EDUCAÇÃO<br/>PROJETO MAIS MÉDICOS PARA O BRASIL</h2>';
					$txt .= '<font size="3"><b>Dados do Formulário</b><br/>'.$rsForm['dsformulario'].'</font></center><br/>';
					$nuGrupo=0;
					$arDimensao = array();
					$arGrupo = array(); 
					$arPergunta = array();
					foreach($rsFormItens as $itens){
						if(!in_array($itens['dsdimensao'], $arDimensao)){

							$txt .= '<div style="background:#f0f0f0;margin-botton:15px;"><p><b><span style="font-size:15px">'.$itens['dsdimensao'].'</span></b></p></div>';
							 
							array_push($arDimensao, $itens['dsdimensao']);
							$nuGrupo = 0;
						}
						if(!in_array($itens['dsgrupo'], $arGrupo)){
							
							++$nuGrupo;

							$txt .= '<div style="padding:0px;">';
							$txt .= '<b><span style="font-size:14px">'.$nuGrupo.'. '.$itens['dsgrupo'].'</span></b>';
							$txt .= '</div>';
							
							$nuPergunta=1;
							array_push($arGrupo, $itens['dsgrupo']);
						}
						if(!in_array($itens['dsgrupo'].' - '.$itens['dspergunta'], $arPergunta)){

							if(strpos( strtolower($itens['dspergunta']), 'cpf' ) !== false) $itens['dsresposta'] = formatar_cpf($itens['dsresposta']);
							if(strpos( strtolower($itens['dspergunta']), 'data' ) !== false) $itens['dsresposta'] = formata_data($itens['dsresposta']);

							$txt .= '<div style="padding-left:15px;">';
							$txt .= '<span style="font-size:13px;">'.$nuGrupo.'.'.$nuPergunta.'. '.strtoupper($itens['dspergunta']).'</span><br/>';
							$txt .= '<i><span style="font-size:11px">'.(empty($itens['dsresposta']) ? 'Sem resposta' : $itens['dsresposta']).'</span></i>';
							$txt .= '</div>';
							
							array_push($arPergunta, $itens['dsgrupo'].' - '.$itens['dspergunta']);
							$nuPergunta++;
						}
					}
					
					ob_clean();
					
					$content = http_build_query(array(
							'conteudoHtml' => utf8_encode($txt)
					));
					
					$context = stream_context_create(array(
							'http' => array(
									'method' => 'POST',
									'content' => $content
							)
					));
					
					$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
					$id = (integer) $_REQUEST['id'];
					header('Content-Type: application/pdf');
					header("Content-Disposition: attachment; filename=Respostas_formulario_".$id.'_'.date('YmdHis'));
					echo $contents;
					exit();

				}
				?>
	
		</div>
		
	</body>
	
</html>