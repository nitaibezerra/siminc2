<? 
/*******************************
* 	CONFIGURAÇÃO
*	Includes e Instancias
*******************************/
	include 'config.inc';
	include APPRAIZ . 'includes/classes_simec.inc';
	include APPRAIZ . 'includes/funcoes.inc';
	$db = new cls_banco();
	
/*******************************
* 	VARIAVEIS
* 	Recebe dados da página Pai
* 	Recebe dados do formulario para
* 	edição e insersção dos dados.
*******************************/
	$codAcaoRequest	= $_REQUEST['acaid']; // Vem da pagina PAI
	$pi				= $_REQUEST['pliid']; // Vem da pagina PAI
	$pstNum			= count($_POST);
	$pstDescricao	= $_POST["descricaopi"];
	$pstObjetivo	= $_POST["objetivopi"];
	$pstGestora		= $_POST["gestora"];
	$pstDotacao		= str_replace(',', '.', $_POST["dotacao"]);
	$pstModalidade	= $_POST["modalidade"];
	$pstConvencao	= $_POST["convencao"];
	$pstPtres		= (string) $_POST["ptres"];

if ($_POST["ptres"]) {	
	while (strlen($pstPtres) < 6)
		$pstPtres = '0'.$pstPtres; 
}
	
/*******************************
* 	AÇÃO RECUPERA DADOS 
*	Recupera os dados para ser 
* 	mostrado na tela. 
*******************************/
if($codAcaoRequest){
	$whereDados = "WHERE ac.acaid =".$codAcaoRequest;
}else if($pi){
	//Monta SQLs 
	$select				= "	pi.pliorcamento as dotacao,
							pi.pliobjetivo as objetivo, 
							pi.plidsc as descricaopi, 
							pi.tpcid as convencao, 
							pi.tpmid as modalidade,
							pi.pliptres, 
							pi.ungcod as gestora,";
	$innerDados 		= " INNER JOIN financeiro.planointerno pi using (acaid)";
	$whereDados 		= " WHERE pi.pliid =".$pi;
	
}

$sql="	SELECT 
			".$select."
			ac.acaid as idacao,
			u.unicod,
			acacod as codigoacao,
			ac.loccod as codigounidade,
			ac.prgcod || '.' || ac.acacod || '.' || ac.unicod || '.' || ac.loccod || ' - ' || ac.acadsc AS descricaoacao, 
			ac.loccod || '- '|| u.unidsc AS unidade
		FROM monitora.acao ac
		INNER JOIN unidade u on u.unicod = ac.unicod
		".$innerDados
		 .$whereDados;

$dados = $db->carregar($sql);
foreach($dados as $dados){
	$idAcao  		= $dados["idacao"];
	$codAcao 		= $dados["codigoacao"];
	$descricaoAcao 	= $dados["descricaoacao"];
	$codUnidade  	= $dados["codigounidade"];
	$unidade   		= $dados["unidade"];
	$dotacao   		= str_replace('.',',',$dados["dotacao"]);
	$convencaoAtual = $dados["convencao"];
	$modalidadeAtual= $dados["modalidade"];
	$gestoraAtual   = $dados["gestora"];
	$objetivo   	= $dados["objetivo"];
	$descricaoPi   	= $dados["descricaopi"];
	$unicod			= $dados["unicod"];
	$ptres			= $dados['pliptres'];
}

if($pi){
	$whereGestora 		= " WHERE ug.ungcod =".$gestoraAtual;
	$whereModalidade 	= " WHERE tpmid = ".$modalidadeAtual;
	$whereConvencao 	= " WHERE tpcid =".$convencaoAtual;
}
/********************** Recupera combo gestora **********************/
$sqlgestora = "select 	ug.ungcod as codigo, 
				ug.ungcod || '.' || ungdsc as gestora  
		from 	unidade u
		inner join unidadegestora ug on ug.unicod = u.unicod and ug.unitpocod = 'U' ";
$gestora = $db->carregar($sqlgestora);

/********************** Recupera combo Modalidade **********************/
$sqlModalidade = "select tpmid as id, tpmcod as codigo ,tpmdsc as descricao from financeiro.tipomodalidade";
$modalidade = $db->carregar($sqlModalidade);

/********************** Recupera combo Convenção **********************/
$sqlConvencao = "select tpcid as id, tpccod as codigo ,tpcdsc as descricao from financeiro.tipoconvencao";
$convencao = $db->carregar($sqlConvencao);


/*******************************
* 	AÇÃO
*	Inserir e editar dados
*******************************/

/********************** Monta codigoPI **********************/
$codUnidadeUltDigitos 	= substr($codUnidade,-3,3); 
$codGestoraUltDigitos 	= substr($pstGestora,-3,3);
$sqlSeguencial 			= "select count(pliid) as seguencia from financeiro.planointerno where acaid =".$idAcao;
$seguencial 			= $db->pegaUm($sqlSeguencial);
$seguencial 			= strlen($seguencial)== 1 ? $seguencial = '0'.$seguencial : $seguencial;
$ultimoDadoPI 			= $codGestoraUltDigitos ? $codGestoraUltDigitos : $codUnidadeUltDigitos;
$pstDotacao 			= str_replace(",", "", $pstDotacao);

if($pstNum && $codAcaoRequest){
	$sqlConvencao 	= "select tpccod as codigo from financeiro.tipoconvencao WHERE tpcid =".$pstConvencao;
	$codConvencao 	= $db->pegaUm($sqlConvencao);
	$sqlModalidade 	= "select tpmcod as codigo from financeiro.tipomodalidade WHERE tpmid = ".$pstModalidade;
	$codmodalidade 	= $db->pegaUm($sqlModalidade);

	$codigoPI = $codAcao.$codmodalidade.$codConvencao.$seguencial.$ultimoDadoPI;
	//echo "codigo acao = ".$codAcao."<br> modalidade = ".$codmodalidade."<br> convencao = ".$codConvencao."<br> seguencial = ".$seguencial."<br> gestor = ".$ultimoDadoPI;
	//exit;
	$sql="	INSERT INTO financeiro.planointerno (plidsc, plicod, pliobjetivo, acaid, pliorcamento, tpcid, tpmid, ungcod, pliptres ) 
			VALUES ('".$pstDescricao."','".$codigoPI."','".$pstObjetivo."','".$idAcao."','".$pstDotacao."','".$pstConvencao."','".$pstModalidade."','".$pstGestora."','".$pstPtres."')";
	$db->executar( $sql );
	$alerta = "Plano Interno incluido com sucesso.";
	
}

if($pstNum && $pi){
	$sql="	UPDATE financeiro.planointerno SET 	plidsc 			= '".$pstDescricao."',
												pliobjetivo 	= '".$pstObjetivo."',
												pliorcamento 	= '".$pstDotacao."',
												pliptres		= '".$pstPtres."'
			WHERE 	pliid = ".$pi;
	$db->executar( $sql );
	$alerta = "Plano Interno alterado com sucesso.";
}
if($pstNum){
	$db->commit();
	echo "<script> 
			alert( '".$alerta."');
			window.opener.location.replace(window.opener.location+'&uo=".$unicod."');
			window.close();
    	  </script>";
}
?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<script language="javascript" type="text/javascript" src="../../includes/funcoes.js"></script>
</head>
<body <?if($pi){ ?>onload="bloquear();"<? }; ?> marginheight="0" marginwidth="0" topmargin="0" leftmargin="0">
<?php
monta_titulo( 'Plano Interno', '<img border="0" src="/imagens/obrig.gif"/> Indica Campo Obrigatório.'  );
?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post" >
		<tr>
			<td class="SubTituloDireita">Ação:</td>
			<td>
				<?=$descricaoAcao;?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Unidade:</td>
			<td>
				<?=$unidade;?>
			</td>
		</tr>
		<? 
		if($codAcao){
			mostra_resp2($idAcao, 'acaid');
		} 
		?>
		<tr>
			<td class="SubTituloDireita">Descrição do Plano Interno:</td>
			<td>
				<input style="width:320px;" type="text" name="descricaopi" id="descricaopi" value="<?=$descricaoPi ?>" /> <img border='0' src='../../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Objetivo do Plano Interno:</td>
			<td>
				<input style="width:320px;" type="text" name="objetivopi" id="objetivopi" value="<?=$objetivo ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Gestora:</td>
			<td>
			<select name="gestora" id="gestora">
				<option  value="">-------------------------</option>
			<?
			if($gestora){
				foreach($gestora as $dadosgestora){
					$codigo		 	= $dadosgestora["codigo"];
					$dadosgestora 	= $dadosgestora["gestora"];
					if($gestoraAtual == $codigo ){
						$opcao="selected";
					}else if ($gestoraAtual !== $codigo){
						$opcao= "";
					}
			?>
				<option  value="<?=$codigo?>" <?=$opcao;?> ><?=$dadosgestora?></option>
			<?
				}
			}
			?>
			</select>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nível / Modalidade:</td>
			<td>
			<select name="modalidade" id="modalidade">
			<option  value="">-------------------------</option>
			<?
			foreach($modalidade as $dadosModalidade){
				$codigo 	= $dadosModalidade["id"];
				$descricao 	= $dadosModalidade["descricao"];
				if($modalidadeAtual == $codigo ){
					$opcao="selected";
				}else if ($modalidadeAtual !== $codigo){
					$opcao= "";
				}
			?>
				<option  value="<?=$codigo?>" <?=$opcao;?> ><?=$descricao?></option>
			<?
			}
			?>
			</select> <img border='0' src='../../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Convenção:</td>
			<td>
			<select name="convencao" id="convencao">
			<option  value="">-------------------------</option>
			<?
			foreach($convencao as $dadosconvencao){
				$codigo 	= $dadosconvencao["id"];
				$descricao 	= $dadosconvencao["descricao"];
				if($convencaoAtual == $codigo ){
					$opcao="selected";
				}else if ($convencaoAtual !== $codigo){
					$opcao= "";
				}
			?>
				<option  value="<?=$codigo?>" <?=$opcao;?> ><?=$descricao?></option>
			<?
			}
			?>
			</select> <img border='0' src='../../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Ptres:</td>
			<td>
				<input style="width:157px;" maxlength="6" type="text" name="ptres" id="ptres" value="<?=$ptres; ?>" onkeyup="this.value = mascaraglobal('######',this.value);" /> <img border='0' src='../../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>		
		<tr>
			<td class="SubTituloDireita">Dotação: <b>R$</b></td>
			<td>
				<input style="width:320px;" onkeyup="this.value = mascaraglobal('##############,##',this.value);" type="text" name="dotacao" id="dotacao" value="<?=$dotacao; ?>" /> <img border='0' src='../../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
		</tr>
		<?if($pi != ""){ ?>
		<tr >
			<td >Extrato PI</td>
		</tr>
		<tr>
			<td colspan=2 style="border-top: 2px solid #000000; border-bottom: 2px solid #000000;" >
				<?
				$sql = "
						SELECT
						 to_char(max(rofdata),'dd/mm/YYYY') as dataatu,
						 pi.plicod || ' ',
						 pi.plidsc,		
						 COALESCE( sum(rofautorizado), 0 ) / 1 AS rofautorizado,
						 COALESCE( sum(rofempenhado), 0 ) / 1 AS empenhado,
						 COALESCE( sum(rofliquidado_favorecido), 0 ) / 1 AS rofliquidado_favorecido,
						 COALESCE( sum(rofpago), 0 ) / 1 as rofpago,
						 CASE WHEN sum( COALESCE( rofautorizado, 0 )) > 0
						  THEN 
						   TRIM(to_char(( sum( COALESCE( rofpago, 0 ) ) * 100 ) / sum( COALESCE( rofautorizado, 0 ) ), '999' ) || ' %')
						  ELSE 		
						   '0 %' 
						 END AS autorizado_porcentagem
						FROM
						 financeiro.planointerno pi 
						 INNER JOIN monitora.acao ac ON pi.acaid = ac.acaid	
						 LEFT JOIN financeiro.execucao ex ON ex.plicod = pi.plicod AND
										     ex.ptres  = pi.pliptres AND
										     ex.acacod = ac.acacod AND 
										     ex.unicod = ac.unicod AND
										     ex.loccod = ac.loccod AND
				 						     ex.prgcod = ac.prgcod AND
										     ex.rofano = ac.prgano
						WHERE 
						 ac.acaid = ".$idAcao."	
						 and
						 pi.pliid = ".$pi."
						GROUP BY
						 pi.plicod,
						 pi.plidsc";
				
					$cabecalho = array(
						'Data <BR>Atualização','Código PI', 'Descrição PI',
						'Lei + Créditos<br>(Autorizado)', 'Empenhado', 'Liquidado', 'Pago', '% do Pago s/<br>Autorizado'
					);
					
					$db = new cls_banco();
					
					$db->monta_lista(
						$sql,
						$cabecalho,
						300,
						20,
						'S',
						'',
						''
					);
								
				?>
			</td>
		</tr>
		<?}?>
		<tr>
		<th colspan="2"> 
			<input type="button" name="gravar" value="gravar" onclick="gravaDados();"/>
			<input type="button" name="fechar" value="fechar" onclick="fecharTela();"/>
		</th>
		</tr>
	</form>
</table>
</body>
</html>
<script type="text/javascript">
	
	/*Submete os dados.*/
	function gravaDados() {
		convencao	= document.getElementById('convencao').value;
		modalidade	= document.getElementById('modalidade').value;
		descricaopi	= document.getElementById('descricaopi').value;
		dotacao		= document.getElementById('dotacao').value;
		ptres		= document.getElementById('ptres').value;
		
		mensagem	="";
		if(!document.getElementById("gestora").disabled){
			if(convencao == "" ){
				mensagem += "convenção, \n";
			}
			if(modalidade == "" ){
				mensagem += "modalidade, \n";
			}
		}
		if(descricaopi == "" ){
			mensagem += "descrição do Plano Interno, \n";
		}
		if(dotacao == "" ){
			mensagem += "dotação, \n";
		}
		if (ptres == "") {
			mensagem += "Ptres \n";
		}
		if(mensagem ==""){
			document.formulario.submit();
		}else{
			alert("Os campos " + mensagem + " são obrigatório(s)");
			return false;
		}
	}
	
	/*Bloqueia campo*/
	function bloquear(){
		 document.getElementById("gestora").disabled=true
		 document.getElementById("convencao").disabled=true
		 document.getElementById("modalidade").disabled=true
	}
	
	/*Fecha tela poupoup*/
	function fecharTela(){
		window.close();
	}
</script>

<?
/*******************************
* 	MOSTRA COORDENADOR DA AÇÃO
*	Inserir e editar dados
*******************************/
function mostra_resp2( $chave_valor, $chave_nome,$ano=true,$schema='monitora' )
{
		$db = new cls_banco();
	// carrega os registros
	if ($ano){
	$sql = sprintf(
		"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p 
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf 
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' and ur.prsano = '%s'
				order by p.pflnivel",
	$chave_nome,
	$chave_valor,
	$_SESSION['exercicio']
	);
	

	}else
	$sql = sprintf(
		"select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p 
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf 
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' 
				order by p.pflnivel",
	$chave_nome,
	$chave_valor
	);
	$responsaveis = $db->carregar( $sql );
	if ( !$responsaveis ) {
		return;
	}

	// exibe o primeiro registro
	echo "<script>
				function exibirEquipeApoio(){
					elemento = document.getElementById( 'responsaveis' );
					imagem = document.getElementById( 'botao_mais_menos' );
					if ( elemento.style.display == 'block' ) {
						elemento.style.display = 'none';
						imagem.src = '../../imagens/mais.gif';
					} else {
						imagem.src = '../../imagens/menos.gif';
						elemento.style.display = 'block';
					}
				}
			</script>";
	$responsavel = array_shift( $responsaveis );
	$htm = sprintf(
		"<tr><td width='250' align='right' class='SubTituloDireita'><a href='#' title='exibir equipe de apoio' onclick='exibirEquipeApoio();'><img id='botao_mais_menos' src='../../imagens/mais.gif' border='0'/></a> %s:</td><td><img src='../../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
	$responsavel['pfldsc'],
	$responsavel['usucpf'],
	$responsavel['usunome'],
	$responsavel['unidsc'],
	$responsavel['usufoneddd'],
	$responsavel['usufonenum']
	);
	echo $htm;

	// exibe os demais registros
	echo '<tr><td colspan="2" style="border: 0; padding: 0;"><div id="responsaveis" style="display: none"><table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; border: 0">';
	foreach ( $responsaveis as $indice => $responsavel ) {
		// monta o html
		$htm = sprintf(																					
			"<tr><td width='250' align='right' class='SubTituloDireita' width='20%%'>%s:</td><td><img src='../../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
		$responsavel['pfldsc'],
		$responsavel['usucpf'],
		$responsavel['usunome'],
		$responsavel['unidsc'],
		$responsavel['usufoneddd'],
		$responsavel['usufonenum']
		);
		echo $htm;
	}
	echo "</table></div></td></tr>";
	
}

?>