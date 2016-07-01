<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];
$andDataLimite = " and a.avldtavaliacao > '2013-12-31'";
if(!$_SESSION['pse']['sse'] || !$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?php
	exit;
}

//recupera o idebp
$dadosEscola = $db->pegaLinha("select esc.idebp, ideqtcrechepac, ideqtpreescolapac, ideqtfundamentalpac, idetotalpactuado, 
                                                           adsqtpaccreche, adsqtpacpreescola, adsqtpacfundamental, adsqtpacmedio, adsqtpaceja,
							   ideqtmediopac, ideqtejapac, idetotaleducando, ede.muncod, ade.estuf
							   from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
							   left join entidade.endereco ede on ede.entid = ent.entid
                                                           left join pse.adesaoescola ade on esc.idebp = ade.idebp
							   where ent.entid = {$entid}
                                                           and acaoid = '{$_SESSION['pse']['acaoid']}' and comid = '2'");
                                                         
if($dadosEscola) extract($dadosEscola);
if(!$idebp){
	?>
	<script>
		alert("Essa Escola não possui acesso ao Componente II.");
		//location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?php
	//exit;
}



//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
//if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';



//excluir registro
if($_REQUEST['exluir'] && $_REQUEST['idave']){
	
	
	$sql = "DELETE FROM pse.scnesavaliaescola WHERE idave = ".$_REQUEST['idave'];
	$db->executar($sql);
	
	$sql = "DELETE FROM pse.avaliaescola WHERE idave = ".$_REQUEST['idave'];
	$db->executar($sql);		
				
	$db->commit();
	
	print "<script>
		alert('Operação realizada com sucesso!');
		location.href='pse.php?modulo=principal/cadastroComp2&acao=A';
	   </script>";
	
	exit();
}





//submit
if($_POST){
	
	if(!$_POST['idebp']){
		?>
		<script>
			alert("Sua escola não está na base do PSE. Favor entrar em contato com o Gestor do sistema.");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?php
		exit;	
	}
	//verifica o somatorio do Número de educandos pactuados (avleducandoavaliado)

	if($_POST['idave']) $andSoma = " and idave not in ({$_POST['idave']})";
	$sql = "select COALESCE(sum(avlqtrealizada),0) as total 
			from pse.avaliaescola 
			where idebp = {$_POST['idebp']}
			and acaoid = {$_SESSION['pse']['acaoid']}
			and avlqtpactuada = ".($_POST['avlqtpactuada'] ? $_POST['avlqtpactuada'] : 0)."
			$andSoma";

	$somaAvaliados = $db->pegaUm($sql);
	if($ideqtcrechepac == $_POST['avlqtpactuada']){
		$moetotalpactuadotipo = $ideqtcrechepac ;
	}elseif($ideqtpreescolapac == $_POST['avlqtpactuada']){
		$moetotalpactuadotipo = $ideqtpreescolapac;
	}elseif($ideqtfundamentalpac == $_POST['avlqtpactuada']){
		$moetotalpactuadotipo = $ideqtfundamentalpac;
	}elseif($ideqtmediopac == $_POST['avlqtpactuada']){
		$moetotalpactuadotipo = $ideqtmediopac;
	}elseif($ideqtejapac == $_POST['avlqtpactuada']){
		$moetotalpactuadotipo = $ideqtejapac;
	}
	if(!$somaAvaliados) $somaAvaliados = 0;
	$somaAvaliados += (int) $_POST['avlqtrealizada'];
	
	//verifica se é numerico
		if($_POST['avlqtpactuada']){
			if(!is_numeric($_POST['avlqtpactuada'])){
				print "<script>
					alert('Erro: Número Quantidade Pactuada inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}
		if($_POST['avlqtrealizada']){
			if(!is_numeric($_POST['avlqtrealizada'])){
				print "<script>
					alert('Erro: Número Quantidade Realizada inválido! Informe o número correto.');
					history.back();
			   	  </script>";
				exit();	
			}		
		}

	//fim verifica
	
	//trata percentual
	if($_POST['avlperctrealizao']){
		$v = str_replace(',','.',$_POST['avlperctrealizao']);
		if($v>999) $_POST['avlperctrealizao'] = 999;
	}
		
	$idave = $_POST['idave']; 
	
	if(!$idave) { //INSERIR
		
		$sql = "INSERT INTO pse.avaliaescola
			(
            	idebp,
			  	acaoid,
			  	comid,
			  	iduf,
			  	muncod,
			  	idnivel,
			  	avlqtpactuada,
			  	avlqtrealizada,
			  	avlperctrealizao,
			  	avlavalsseoumonit,
			  	avldtavaliacao,
                                avldtregistro,
                                usucpf
	        ) VALUES (
	        	".$_POST['idebp'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	'".$_POST['estuf']."',
	        	'".$_POST['muncod']."',
	        	".$_POST['idnivel'].",
	        	".($_POST['avlqtpactuada'] ? $_POST['avlqtpactuada'] : 'null').",
	        	".($_POST['avlqtrealizada'] ? $_POST['avlqtrealizada'] : 'null').",
	        	".($_POST['avlperctrealizao'] ? str_replace(',','.',$_POST['avlperctrealizao']) : 'null').",
	        	'".$_POST['avlavalsseoumonit']."',
	        	'".formata_data_sql($_POST['avldtavaliacao'])."',
                        NOW(),
                        '".$_SESSION['usucpf']."'
	        ) returning idave";	
                //ver($sql,d);
             
		$idave = $db->pegaUm($sql);
		
	}
	else{ //ALTERAR
		
		$sql = "UPDATE 
					pse.avaliaescola
				SET 
					idnivel = ".$_POST['idnivel'].",
					avlqtpactuada = ".($_POST['avlqtpactuada'] ? $_POST['avlqtpactuada'] : 'null').",
					avlqtrealizada = ".($_POST['avlqtrealizada'] ? $_POST['avlqtrealizada'] : 'null').",
					avlperctrealizao = ".($_POST['avlperctrealizao'] ? str_replace(',','.',$_POST['avlperctrealizao']) : 'null').",
					avlavalsseoumonit = '".$_POST['avlavalsseoumonit']."',
					avldtavaliacao = '".formata_data_sql($_POST['avldtavaliacao'])."'
				WHERE 
					idave = ".$idave;
		$db->executar($sql);
	
	}
	
	//inserir equipe ESF
	$sql = "DELETE FROM pse.scnesavaliaescola WHERE idave = ".$idave;
	$db->executar($sql);
	
	if($_POST['cneid'][0]){
		foreach($_POST['cneid'] as $v){
			$sql = "INSERT INTO pse.scnesavaliaescola (idave, cneid) 
					VALUES (".$idave.",".$v.")";
			$db->executar($sql);
		}
	}	
				
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroComp2&acao=A';
		   </script>";
	exit();
	
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => $_SERVER['REQUEST_URI']),
			  2 => array("id" => 3, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
*/


if($_SESSION['pse']['sse'] == 'S'){
	$titulo = "Monitoramento das Ações da Semana Saúde na Escola";
}elseif($_SESSION['pse']['sse'] == 'N'){
	$titulo = "Monitoramento das Ações do PSE na Escola";
}elseif($_SESSION['pse']['sse'] == 'B'){
	$titulo = "Monitoramento das Ações do Brasil Carinhoso";
}

$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);

echo "<br>";

//$db->cria_aba( $abacod_tela, $url, '');

$menu = array(0 => array("id" => 1, "descricao" => "Lista de Ações", 			"link" => "/pse/pse.php?modulo=principal/monitoraComp2&acao=A"),
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => $_SERVER['REQUEST_URI'])
			  //2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A"),
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

// ver($_SESSION,d);
//carrega dados para edição
if($_REQUEST['idave']){
	$sql = "SELECT  
				  idave,
                                  idnivel,
				  avlqtpactuada,
				  avldtavaliacao,
				  avlqtrealizada,
				  avlperctrealizao,
				  avlavalsseoumonit
		     FROM 
		     		pse.avaliaescola
		     WHERE 
		     		idave = {$_REQUEST['idave']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}



?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="idebp" value="<?=$idebp?>">
<input type="hidden" name="idave" value="<?=$idave?>">

<input type="hidden" name="estuf" value="<?=$estuf?>">
<input type="hidden" name="muncod" value="<?=$muncod?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td style="background: rgb(168, 168, 168);">&nbsp;</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	COMPONENTE II - PROMOÇÃO DA SAÚDE E PREVENÇÃO
				     	<br>
				     </b>
				     </font>
				     <br>
				</td>
			</tr>
			<tr>
				<td style="background: rgb(168, 168, 168);">&nbsp;</td>
			</tr>
	    </table>
	</td>
</tr>
<tr>
	<td align="center" style="background-color: #ffffff">
		<table width="100%" align="center" cellpadding="2" cellspacing="2">
			<tr>
				<td colspan='4' class="SubTituloCentro" align="center">
					<b>
						Ação: <?=$db->pegaUm("select acaodescricao from pse.acao where acaoid = ".$_SESSION['pse']['acaoid']);?>	
					</b>
				</td>
			</tr>
			<tr>
				<td class="SubTituloCentro">
					Total de educandos da escola: 
					<?php
						/*
						if($entid){
							$sql = "select pacquantedutotal, pacquanteduseratend  
									from pse.planoacaoc2
									where entid = $entid";
							$comp2 = $db->pegaLinha($sql);
							$moetotaleducando = ($comp2['pacquantedutotal'] ? $comp2['pacquantedutotal'] : 0);
							$moetotalpactuado = ($comp2['pacquanteduseratend'] ? $comp2['pacquanteduseratend'] : 0);
							if($moetotalpactuado>0){
								$moetotalavaliado = ($moetotaleducando * 100) / $moetotalpactuado;
							}else{
								$moetotalavaliado = 0;
							}
							
						}
						*/
						$moetotaleducando = $idetotaleducando;
						
						echo campo_texto( 'moetotaleducando', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
				</td>
				<td class="SubTituloCentro">
					Total de educandos pactuados: 
					<?php
                                        $moetotalpactuado = $idetotalpactuado;
						//$moetotalpactuado = ($ideqtcrechepac + $ideqtpreescolapac + $ideqtfundamentalpac + $ideqtmediopac + $ideqtejapac);
						echo campo_texto( 'moetotalpactuado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
					?>
				</td>
                                <td class="SubTituloDireita"  style='display: none;'>
					Total de educandos avaliados: 
					<?php
						$somaAvaliados = 0;
						if($idebp && $_SESSION['pse']['acaoid']){
							$sql = "select sum(avlqtrealizada) as total 
									from pse.avaliaescola a
									where idebp = {$idebp}
                                                                        $andDataLimite
									and acaoid = {$_SESSION['pse']['acaoid']}
									$andSoma";

                                                        $somaAvaliados = $db->pegaUm($sql);
						}
						
						if($moetotalpactuado>0){
							$moetotalavaliado = ($somaAvaliados * 100) / $moetotalpactuado;
							$moetotalavaliado = number_format($moetotalavaliado,2);
						}else{
							$moetotalavaliado = 0;
						}
						
						echo campo_texto( 'moetotalavaliado', 'N', 'N', '', 10, 6, '', '', 'right', '', 0, '');
						echo '%';
					?>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
	<tr>
		<td width="40%" class="SubTituloDireita" colspan='2'>Informe a Equipe da Atenção Básica, caso esta tenha participado da ação:</td>
		<td colspan='2'>
			<?php
				/*
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  --and cnetppopassistescola='A'
							  order by 2";
				*/
			
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  order by 2";
				
				if ($idave){					
					$sql = "SELECT
								a.cneid as codigo,
								a.cnecodigocnes ||' - '|| a.cnenomefantasia ||' - '|| a.cneseqequipe as descricao
							  FROM pse.scnes a
							  inner join pse.scnesavaliaescola s on s.cneid = a.cneid
							  where idave = ".$idave." 
							  --and cnetppopassistescola='A'
							  order by 2";
					//dbg($sql);
					//$nome = 'cneid';
					//$$nome = $db->carregar( $sql ); 
					$cneid = $db->carregar( $sql );
				}
				
				
				combo_popup( 'cneid', $sql_combo, 'Selecione a(s) Equipes(s)', '360x460', '', '', '', $vPermissao, true, '', '5', '400', '', '', false, $where = '', '', $mostraPesquisa = true, $campo_busca_descricao = false, '', false, '' , '' );
				//echo obrigatorio();
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='4'>&nbsp;</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Nível:</td>
		<td colspan='2'>
			<?php
				$sql_combo = "SELECT
								idnivel as codigo,
								niveldescricao as descricao
							  FROM pse.niveldeensino
							  order by 2";			
				$db->monta_combo("idnivel", $sql_combo, $vPermissao, "Selecione o Nível", 'filtraQtd', '', '', '', 'S', 'idnivel');
			?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Quantidade Pactuada:</td>
		<td colspan='2'>
			<?=campo_texto( 'avlqtpactuada', 'N', 'N', '', 9, 6, '######', '', 'right', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Data da Ação:</td>
		<td colspan='2'>
			<?=campo_data('avldtavaliacao', 'S', $vPermissao, '', 'S' );?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Quantidade Realizada:</td>
		<td colspan='2'>
			<?=campo_texto( 'avlqtrealizada', 'S', $vPermissao, '', 9, 6, '######', '', 'right', '', 0, "onkeydown='bloqTab(event);'", '', '', 'calcPerc();');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Percentual Realizado:</td>
		<td colspan='2'>
			<?=campo_texto( 'avlperctrealizao', 'N', 'N', '', 9, 6, '######', '', 'right', '', 0, '');?> %
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Avaliação realizada na Semana Saúde na Escola:</td>
		<td colspan='2'>
			<input type="radio" name="avlavalsseoumonit" value="S" <?if($avlavalsseoumonit == 'S') echo 'checked';?>> SIM
			&nbsp;
			<input type="radio" name="avlavalsseoumonit" value="N" <?if($avlavalsseoumonit == 'N') echo 'checked';?>> NÃO
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormPse();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/monitoraComp2&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC1" style="height: 150px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					if(!$idebp) $idebp = 0;
                                        if ($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA) {
                                                $sql = "SELECT
									'<center>' ||
									(CASE WHEN '{$vPermissao}' = 'S' THEN
											 '<a href=\"#\" onclick=\"alterarComp2(\'' || a.idave || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirComp2(\'' || a.idave || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
										  ELSE
											 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
									END) || '</center>'  as acao,
									'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = {$_SESSION['pse']['comid']}
								and a.acaoid = {$_SESSION['pse']['acaoid']}
								and a.idebp = {$idebp}
                                                                $andDataLimite
								order by 2";

    $cabecalho = array("Ação", "Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
    $alinha = array("center", "left", "center", "center", "center", "center", "center");
    $tamanho = array("10%", "20%", "20%", "20%", "20%", "20%", "20%");
} else {
    $sql = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = {$_SESSION['pse']['comid']}
								and a.acaoid = {$_SESSION['pse']['acaoid']}
								and a.idebp = {$idebp}
                                                                and a.avldtavaliacao > '2014-12-31'    
								order by 2";

    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
    $alinha = array("left", "center", "center", "center", "center", "center");
    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
}

						$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
				?>
				
				
			<div>
		</td>
	</tr>
</table>


</form>

</body>
</html>

 
<script>

var d = document.formulario;

function bloqTab(evt){
	/*
	if (e.keyCode) code = e.keyCode; 
	else if (e.which) code = e.which; // Netscape 4.? 
	else if (e.charCode) code = e.charCode; // Mozilla
	*/
	var charCode = (evt.which) ? evt.which : event.keyCode;
	//var keyCode = event.keyCode ? event.keyCode : event.which ? event.which : event.charCode;
	if(charCode == 9) {
		d.avlperctrealizao.focus();
		//alert('teste');
		//d.avlqtrealizada.value = d.avlqtrealizada.value.replace("	","");
		//calcPerc();
		//return false;
	}
	
}

function filtraQtd(id){   
	
	if(id==1){
		d.avlqtpactuada.value = "<?=$adsqtpaccreche?>";
	}
	else if(id==2){
		d.avlqtpactuada.value = "<?=$adsqtpacpreescola?>";
	}
	else if(id==3){
		d.avlqtpactuada.value = "<?=$adsqtpacfundamental?>";
	}
	else if(id==4){
		d.avlqtpactuada.value = "<?=$adsqtpacmedio?>";
	}
	else if(id==5){
		d.avlqtpactuada.value = "<?=$adsqtpaceja?>";
	}
	
	if(!d.avlqtpactuada.value) d.avlqtpactuada.value = 0;
	
	calcPerc();
}

function calcPerc(){

	var avlqtpactuada = d.avlqtpactuada.value;
	var avlqtrealizada = trim(d.avlqtrealizada.value);
	var porcento = 0;
	
	if(!avlqtpactuada) avlqtpactuada = 0;
	if(!avlqtrealizada) avlqtrealizada = 0;
	
	if(parseFloat(avlqtpactuada)>0){
		porcento = ( parseFloat(avlqtrealizada) * 100 ) / parseFloat(avlqtpactuada);
	}else{
		porcento = 0;
	}
	
	d.avlperctrealizao.value = porcento;
	
}

function validaFormPse(){

	/*
	if(document.getElementById('cneid')[0].value == ''){
		alert('Informe as Equipes da Atenção Básica.');
		d.cneid.focus();
		return false;
	}	
	*/
	
	if(d.idebp.value == ''){
		alert('Sua escola não está na base do PSE. Favor entrar em contato com o Gestor do sistema.');
		return false;
	}
	if(d.idnivel.value == ''){
		alert('Favor Preencher o campo obrigatório: Nível.');
		d.idnivel.focus();
		return false;
	}
	if(d.avldtavaliacao.value == ''){
		alert('Favor Preencher o campo obrigatório: Data da ação.');
		d.avldtavaliacao.focus();
		return false;
	}
	if(d.avlqtrealizada.value == ''){
		alert('Favor Preencher o campo obrigatório: Quantidade Realizada.');
		d.avlqtrealizada.focus();
		return false;
	}
	if(d.avlavalsseoumonit[0].checked == false && d.avlavalsseoumonit[1].checked == false){
		alert('Favor Preencher o campo obrigatório: Avaliação realizada na Semana Saúde na Escola.');
		d.avlavalsseoumonit[0].focus();
		return false;
	}
		
	
	var dtavaliacao = d.avldtavaliacao.value.substr(6,4) + d.avldtavaliacao.value.substr(3,2) + d.avldtavaliacao.value.substr(0,2);
	var dtatual = "<?=date('Ymd')?>";
	
	if( parseFloat(dtavaliacao) >  parseFloat(dtatual) ){
		alert('Data da ação não pode ser maior que a data de hoje.');
		//d.avldataavaliacao.focus();
		return false;
	}
	
	if( parseFloat(dtavaliacao) <  parseFloat('20130101') ){
		alert('Data da ação não pode ser menor que 01/01/2013.');
		//d.avldataavaliacao.focus();
		return false;
	}

	selectAllOptions( document.getElementById( 'cneid' ) );
	d.submit();

}


function alterarComp2(id){
	location.href='pse.php?modulo=principal/cadastroComp2&acao=A&idave='+id;
}

function excluirComp2(id){

	if(confirm('Deseja realmente excluir este item?')){
		location.href='pse.php?modulo=principal/cadastroComp2&acao=A&exluir=1&idave='+id;
	}
}
</script>