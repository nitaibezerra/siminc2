<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];

if(!$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
}

//recupera o idebp
$dadosEscola = $db->pegaLinha("select idebp, ideqtcrechepac, ideqtpreescolapac, ideqtfundamentalpac,
							   ideqtmediopac, ideqtejapac, ede.muncod, ede.estuf
							   from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
							   left join entidade.endereco ede on ede.entid = ent.entid
							   where ent.entid = {$entid}");
if($dadosEscola) extract($dadosEscola);
 
if(!$idebp){
	?>
	<script>
		alert("Essa Escola não possui acesso ao Componente II.");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
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



//submit
if($_POST){
	
	if(!$_POST['idebp']){
		?>
		<script>
			alert("Sua sessão expirou. Por favor, entre novamente!");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;	
	}
	
	$psmid = $_POST['psmid']; 
		
	if(!$psmid) { //INSERIR
		
		$sql = "INSERT INTO pse.psmnaescola
			(
            	idebp,
			  	acaoid,
			  	comid,
			  	estuf,
			  	muncod,
			  	statusacao
	        ) VALUES (
	        	".$_POST['idebp'].",
	        	".$_SESSION['pse']['acaoid'].",
	        	".$_SESSION['pse']['comid'].",
	        	'".$_POST['estuf']."',
	        	'".$_POST['muncod']."',
	        	'".($_POST['statusacao'] == 't' ? 't' : 'f')."'
	        ) returning psmid";	
		$psmid = $db->pegaUm($sql);
		
	}
	else{ //ALTERAR
		
		$sql = "UPDATE 
					pse.psmnaescola
				SET 
					statusacao = '".($_POST['statusacao'] == 't' ? 't' : 'f')."'
				WHERE 
					psmid = ".$psmid;
		$db->executar($sql);
	
	}
	
	//inserir nivel
	$sql = "DELETE FROM pse.psmnivel WHERE psmid = ".$psmid;
	$db->executar($sql);
	
	if($_POST['statusnivel'] && $_POST['statusacao'] == 't'){
		foreach($_POST['statusnivel'] as $key => $value){
			$sql = "INSERT INTO pse.psmnivel (psmid, idnivel, statusnivel) 
					VALUES (".$psmid.",".$key.",'".$value."')";
			$db->executar($sql);
		}
	} 
	
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroComp21&acao=A';
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


//carrega dados para edição
if($idebp){
	$sql = "SELECT  
				idebp,
			  	psmid,
			  	statusacao
		     FROM 
		     		pse.psmnaescola
		     WHERE 
		     		idebp = {$idebp}
		     and 
		     		comid = {$_SESSION['pse']['comid']}
		     and 
		     		acaoid = {$_SESSION['pse']['acaoid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}



?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="idebp" value="<?=$idebp?>">
<input type="hidden" name="psmid" value="<?=$psmid?>">

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
				     	<br>
				     	<font color="black">Ação - Promoção da Saúde Mental no território escolar</font> 
				     	<br>
				     </b>
				     </font>
				     <br>
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
		<td class="SubTituloEsquerda">
			<?
			if($_SESSION['pse']['acaoid'] == 27){
				echo ' - A escola e a equipe de atenção básica criaram grupos intersetoriais de discussão de ações de saúde mental no contexto escolar, em articulação com o GTI municipal?';
			}elseif($_SESSION['pse']['acaoid'] == 32){
				echo ' - A escola e a equipe de atenção básica realizaram mapeamento na escola das situações de risco e de acidentes?';
			}elseif($_SESSION['pse']['acaoid'] == 33){
				echo ' - A escola e a equipe de atenção básica criaram grupos de famílias solidárias para encontro e troca de experiência, com mediação da creche/escola e/ou saúde?';
			}elseif($_SESSION['pse']['acaoid'] == 34){
				echo ' - A escola e a equipe de atenção básica criaram grupos entre pares para fomento e estímulo ao protagonismo de adolescentes e jovens para administrar conflitos no ambiente escolar?';
			}
			?>
		</td>
		<td width="60%">
			<input type="radio" name="statusacao" value="t" <?if($statusacao == 't') echo 'checked';?> onclick="veropcao(1)"> SIM
			&nbsp;
			<input type="radio" name="statusacao" value="f" <?if($statusacao == 'f') echo 'checked';?> onclick="veropcao(2)"> NÃO
		</td>
	</tr>
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
</table>	
<div id="divnivel" style="display: <?if($statusacao != 't' || $_SESSION['pse']['acaoid'] == 32) echo 'none';?>;">
	<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td colspan='4'><b>Em quais modalidades?</b></td>
	</tr>
	<?
	$sql = "select idnivel, niveldescricao from pse.niveldeensino order by 1";
	$nivel = $db->carregar($sql);
	
	foreach($nivel as $n){
		
		$checado = "";
		if($psmid){
			$sql = "select statusnivel from pse.psmnivel 
					where psmid = {$psmid}
					and idnivel = {$n['idnivel']}";
			$checado = $db->pegaUm($sql);
		}
		?>
			<tr>
				<td class="SubTituloEsquerda" colspan='2'>
					<input type="checkbox" name="statusnivel[<?=$n['idnivel']?>]" value="t" <?if($checado == 't') echo 'checked';?>>
					- <?=$n['niveldescricao']?>
				</td>
			</tr>
		<?
	}
	?>	
	</table>
</div>
<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormPse();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/monitoraComp2&acao=A&tipo=1';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>


</form>

</body>
</html>

 
<script>

var d = document.formulario;

function validaFormPse(){

	<?if($_SESSION['pse']['acaoid'] != 32){?>
		if(d.statusacao[0].checked == true){

			var campos = document.getElementsByTagName("input");
			var selecionado = 0;
			
			for(i=0; i<campos.length; i++){
				if(campos[i].type == 'checkbox'){
					if(campos[i].checked == true){
						selecionado = 1;
						break;
					}
				}
			}
			
			if(selecionado == 0){
				alert("É necessário informar pelo menos uma modalidade!");
				return false;
			}
		}
	<?}?>

	d.submit();
}

function veropcao(cod){
	
	<?if($_SESSION['pse']['acaoid'] != 32){?>
		if(cod == 1){
			document.getElementById("divnivel").style.display = '';
		}
		else{
			document.getElementById("divnivel").style.display = 'none';
		}
	<?}?>
}



</script>