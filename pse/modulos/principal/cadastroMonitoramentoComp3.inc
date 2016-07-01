<?php


if(!$_SESSION['pse']['muncod']){
	?>
	<script>
		alert("É necessário informar um município\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/listaMunicipiosComp3&acao=A';
	</script>
	<?
	exit;
}


//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == ESCOLA_MUNICIPAL || $vpflcod == ESCOLA_ESTADUAL || $vpflcod == SECRETARIA_ESTADUAL || $vpflcod == SECRETARIA_MUNICIPAL){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}


//apos 31 de novembro, permite so vizualizar os dados
if( date('Ymd') > 20121130 ) $vPermissao = 'N';


if($_POST){
	
	if($_POST['mpaid']){
		$sql = "DELETE FROM pse.avaliacomp3 WHERE mpaid = ".$_POST['mpaid'];
		$db->executar($sql);
		
		$sql = "INSERT INTO pse.avaliacomp3 (mpaid, comid, acaoid, ac3realizacomp3acao)
				VALUES (".$_POST['mpaid'].", 3, 15, '".$_POST['ac3realizacomp3acao1']."')";
		$db->executar($sql);					
		
		$sql = "INSERT INTO pse.avaliacomp3 (mpaid, comid, acaoid, ac3realizacomp3acao)
				VALUES ('".$_POST['mpaid']."', 3, 16, '".$_POST['ac3realizacomp3acao2']."')";
		$db->executar($sql);					
		
		$sql = "INSERT INTO pse.avaliacomp3 (mpaid, comid, acaoid, ac3realizacomp3acao)
				VALUES ('".$_POST['mpaid']."', 3, 17, '".$_POST['ac3realizacomp3acao3']."')";
		$db->executar($sql);					
		
		$db->commit();
	
		unset($_POST);
		print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroMonitoramentoComp3&acao=A';
			   </script>";
	
		exit();
	}
	
}




$sql = "SELECT mundescricao as municipio, estuf as uf
		FROM territorios.municipio
		WHERE muncod = '".$_SESSION['pse']['muncod']."'";
$mun = $db->pegaLinha($sql);

//pega entid da SECRETARIA MUNICIPAL DE EDUCAÇÃO de um determinado municipio
/*
$sql = "select u.entid, e.entnome 
		from seguranca.usuario u
		left join entidade.entidade e on e.entid = u.entid
		where u.usucpf='".$_SESSION['usucpf']."'";
*/
$sql = "select et.entid, et.entnome from entidade.entidade et
		inner join entidade.endereco ed on ed.entid=et.entid
		inner join entidade.funcaoentidade fe on  fe.entid = et.entid AND fe.funid = 7
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];
$conano = $_SESSION["exercicio"];



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "PSE - Monitoramento das Ações - Componente III";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.<br>
<table width="100%" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td width="10%" style="text-align: right;" class="SubTituloDireita">UF:</td>
	<td width="90%" class="SubTituloEsquerda">'.$mun['uf'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">MUNICÍPIO:</td>
	<td class="SubTituloEsquerda">'.$mun['municipio'].'</td>
</tr>
<tr>
	<td style="text-align: right;" class="SubTituloDireita">SECRETARIA:</td>
	<td class="SubTituloEsquerda">'.$secretaria['entnome'].'</td>
</tr>
</table>';

echo "<br>";
/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoPactuacao&acao=A"),
					  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
*/
monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');


if($entid){
	$sql = "SELECT conid FROM pse.contratualizacao WHERE entid = $entid";
	$conid = $db->pegaUm($sql);
	if(!$_SESSION['conid'] && $conid) $_SESSION['conid'] = $conid;
}

if($_SESSION['conid']){
	$sql = "SELECT  m.mpaid,
					m.mpaacao06comp1,
					m.mpaacao01comp3,
					m.mpaacao02comp3,
					m.mpaacao03comp3,
					m.mpaacao04comp3,
					m.mpaacao05comp3
		     FROM 
		     		pse.metapactuada m
		     WHERE 
		     		m.conid = {$_SESSION['conid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
	
	if(!$mpaacao01comp3 || !$mpaacao02comp3 || !$mpaacao03comp3){
		?>
		<script>
			alert("Acesso Negado! \nNa Pactuação do Termo de Compromisso, o Componente III não foi preenchido.");
			location.href='pse.php?modulo=principal/listaMunicipiosComp3&acao=A';
		</script>
		<?
		exit;
	}
	
	
	if($mpaid){
		$sql = "SELECT ac3realizacomp3acao
	  			FROM pse.avaliacomp3 
				WHERE comid = 3 and acaoid = 15 and mpaid = {$mpaid}";
		$ac3realizacomp3acao1 = $db->pegaUm($sql);
		
		$sql = "SELECT ac3realizacomp3acao
	  			FROM pse.avaliacomp3 
				WHERE comid = 3 and acaoid = 16 and mpaid = {$mpaid}";
		$ac3realizacomp3acao2 = $db->pegaUm($sql);
		
		$sql = "SELECT ac3realizacomp3acao
	  			FROM pse.avaliacomp3 
				WHERE comid = 3 and acaoid = 17 and mpaid = {$mpaid}";
		$ac3realizacomp3acao3 = $db->pegaUm($sql);
	}	
	
}

?>

<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="mpaid" value="<?=$mpaid?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>
		<center>
			<span style="font-size: 12"><b>EDUCAÇÃO PERMANENTE</b></span>
		</center>
	</td>
</tr>
<tr>
	<td colspan='4' class="SubTituloEsquerda" valign="top" nowrap>
		 <br>
	     <div style="text-align:center"><span style=" font-family:'Tahoma'; color:#000000; font-size:15px;"><b>Linha de Ação</b></span><br><span style=" font-family:'Tahoma'; color:#000000; font-size:11px;"><b>Educação permanente e capacitação local de profissionais da educação nos temas da saúde e constituição das equipes de saúde que atuarão nos Territórios do Programa Saúde na Escola</b></span></div>
	      <br><br>
	</td>
</tr>

<tr>
	<td width="70%" class="SubTituloCentro">AÇÃO</td>
	<td width="10%" class="SubTituloCentro">META PACTUADA</td>
	<td width="10%" class="SubTituloCentro">REALIZADO</td>
	<td width="10%" class="SubTituloCentro">%</td>
</tr>
<?php 
	$sql = "select laaid, laalinhaacao, laaacao, laacomponente, laastatus
			from pse.complinhaacao 
			where laacomponente = '3'
			and laastatus = 'A' ";
	$dados = $db->carregar($sql);
	$i = 1; 
	foreach($dados as $d){
		
		$porcento01 = '';
		if($mpaacao01comp3>0){
			if($ac3realizacomp3acao1) $porcento01 = ($ac3realizacomp3acao1 * 100) / $mpaacao01comp3;
			if($porcento01) $porcento01 = number_format($porcento01,2,",",".");
		}
		
		$porcento02 = '';
		if($mpaacao02comp3>0){
			if($ac3realizacomp3acao2) $porcento02 = ($ac3realizacomp3acao2 * 100) / $mpaacao02comp3;
			if($porcento02) $porcento02 = number_format($porcento02,2,",",".");
		}
		
		$porcento03 = '';
		if($mpaacao03comp3>0){
			if($ac3realizacomp3acao3) $porcento03 = ($ac3realizacomp3acao3 * 100) / $mpaacao03comp3;
			if($porcento03) $porcento03 = number_format($porcento03,2,",",".");
		}
		?>
		<tr>
			<td class="SubTituloEsquerda"><?=$d['laaacao']?></td>
			<td class="SubTituloCentro" nowrap="nowrap" >
				<?
					echo campo_texto( 'mpaacao0'.$i.'comp3', 'N', 'N', '', 18, 6, '######', '', 'right', '', 0, '', '', '', '');
				?>
			</td>
			<td class="SubTituloCentro" nowrap="nowrap" >
				<?
					echo campo_texto( 'ac3realizacomp3acao'.$i, 'S', 'S', '', 18, 6, '######', '', 'right', '', 0, '', 'calcperc('.$i.')', '', '');
				?>
			</td>
			<td class="SubTituloCentro" nowrap="nowrap" >
				<?
					echo campo_texto( 'porcento0'.$i, 'N', 'N', '', 18, 6, '######', '', 'right', '', 0, '', '', '', '');
				?>
			</td>
		</tr>
		<?
		$i++;
	}
	?>


<tr>
	<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
		<?if($vPermissao == 'S'){?>
			<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaForm();">
		<?}?>
	</td>
</tr>
</table>


</form>

<script type="text/javascript">

var d = document.formulario;

function validaForm(){

	if(d.ac3realizacomp3acao1.value > 0 && d.ac3realizacomp3acao2.value > 0 && d.ac3realizacomp3acao3.value > 0){
		d.submit();
	}
	else{
		alert ('Favor Preencher todos os campos obrigatórios.');
		return false;
	}

}

function calcperc(campo)
{
	
	var perc = 0;
	var valor = 0;
	
	if(campo == '1'){
		valor = d.ac3realizacomp3acao1.value;
		if(d.mpaacao01comp3.value>0){
			perc = (valor * 100) / d.mpaacao01comp3.value;
		}
		if(valor>0){
			d.porcento01.value = float2moeda(perc);
		}
		else{
			d.porcento01.value = '';
		}
	}
	if(campo == '2'){
		valor = d.ac3realizacomp3acao2.value;
		if(d.mpaacao02comp3.value>0){
			perc = (valor * 100) / d.mpaacao02comp3.value;
		}
		if(valor>0){
			d.porcento02.value = float2moeda(perc);
		}
		else{
			d.porcento02.value = '';
		}
	}
	if(campo == '3'){
		valor = d.ac3realizacomp3acao3.value;
		if(d.mpaacao03comp3.value>0){
			perc = (valor * 100) / d.mpaacao03comp3.value;
		}
		if(valor>0){
			d.porcento03.value = float2moeda(perc);
		}
		else{
			d.porcento03.value = '';
		}
	}
	
}


function float2moeda(num) {
	   x = 0;
	   if(num<0) {
	      num = Math.abs(num);
	      x = 1;
	   }
	   if(isNaN(num)) num = "0";
	      cents = Math.floor((num*100+0.5)%100);
	   num = Math.floor((num*100+0.5)/100).toString();
	   if(cents < 10) cents = "0" + cents;
	      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
	         num = num.substring(0,num.length-(4*i+3))+'.'
	               +num.substring(num.length-(4*i+3));
	    ret = num + ',' + cents;
	    if (x == 1) ret = ' - ' + ret;
	    return ret;
}

<?php //if($_SESSION['permissaoedicao'] == false){?>
	/*
	var obj = document.getElementsByTagName("input");
	var total = document.getElementsByTagName("input").length;
	
	for(i=0; i<total; i++){
		obj[i].disabled = true;
	}
	*/
<?php //}?>

</script>