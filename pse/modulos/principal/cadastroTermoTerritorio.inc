<?php

//dbg($_SESSION);
if(!$_SESSION['conid']){
	?>
	<script>
		alert("É necessário preencher a tela 'Cadastrar Termo de Compromisso.'\n ou sua sessão expirou. Por favor, entre novamente no PSE!");
		location.href='pse.php?modulo=principal/cadastroTermoCompromisso&acao=A';
	</script>
	<?
	exit;
}

function alterarDados(){
	global $db;
	
	$vlcomp1 = $_POST['conpercentualcomp1'];
	$vlcomp1 = str_replace('.','',$vlcomp1);
	$vlcomp1 = str_replace(',','.',$vlcomp1);

	$vlcomp2 = $_POST['conpercentualcomp2'];
	$vlcomp2 = str_replace('.','',$vlcomp2);
	$vlcomp2 = str_replace(',','.',$vlcomp2);
	
	$sql = "UPDATE 
				pse.contratualizacao
			SET 
				conquantesfmun = '".$_POST['conquantesfmun']."',
				conquantescolapub = '".$_POST['conquantescolapub']."',
				conquanteducando = ".$_POST['conquanteducando'].",
				conesfatuarapse = '".$_POST['conesfatuarapse']."',
				concoberturacomp1 = '".$_POST['concoberturacomp1']."',
				conpercentualcomp1 = '".$vlcomp1."',
				concoberturacomp2 = '".$_POST['concoberturacomp2']."',
				conpercentualcomp2 = '".$vlcomp2."'
			WHERE 
				conid = ".$_POST['conid'];
	//ver( $sql,d );
	$db->executar($sql);					
	$db->commit();
}






if($_POST){
	
	if($_POST['entid']){
		alterarDados();
		unset($_POST);
		print "<script>
				alert('Operação realizada com sucesso!');
				location.href='pse.php?modulo=principal/cadastroTermoTerritorio&acao=A';
			</script>";
	}
	else{
		print "<script>
					alert('Erro ao gravar os dados! Favor entrar em contato com o administrador do sistema.');
					location.href='pse.php?modulo=principal/cadastroTermoTerritorio&acao=A';
			  </script>";
	}
	//header( "Location: demandas.php?modulo=sistema/apoio/origemDemanda&acao=A" );
	exit();
	
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
		where fe.fuestatus='A' and muncod = '".$_SESSION['pse']['muncod']."'
		order by 1 limit 1";
$secretaria = $db->pegaLinha($sql);

$entid = $secretaria['entid'];
$conano = $_SESSION["exercicio"];



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


$titulo = "PSE - Termo de Compromisso";
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

$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoTerritorio&acao=A"),
					  1 => array("id" => 2, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
				  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
		
monta_titulo( $titulo, $dsctitulo );
echo "<br>";

$db->cria_aba( $abacod_tela, $url, '');


if($entid){
	$sql = "SELECT  conid,
					entid,
	            	conano, 
	            	conesfatuarapse,
					concoberturacomp1,
					conpercentualcomp1,
					concoberturacomp2,
					conpercentualcomp2,
					conquantesfmun,
					conquantescolapub,
					conquanteducando,
					conesfatuarapse 
		     FROM 
		     		pse.contratualizacao
		     WHERE 
		     		entid = $entid";

	$dados = $db->carregar($sql);
	
	if($dados) extract($dados[0]);
	if($conpercentualcomp1) $conpercentualcomp1 = number_format($conpercentualcomp1,2,",",".");
	if($conpercentualcomp2) $conpercentualcomp2 = number_format($conpercentualcomp2,2,",",".");
	
	
}

?>

<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="conid" value="<?=$conid?>">
<input type="hidden" name="entid" value="<?=$entid?>">
<input type="hidden" name="conano" value="<?=$conano?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td class="SubTituloEsquerda" colspan='4'>
		<br>
	     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Mapeamento do território PSE - ESF e Escolas:</b>
	     <br><br>
	</td>
</tr>
<tr>
	<td style="background: rgb(238, 238, 238)" colspan='4'>
		<br>
	     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; O Ministério da Saúde realizará a transferência do recurso financeiro por meio de repasse fundo a fundo na modalidade PAB variável, compondo o Bloco de Financiamento da Atenção Básica do Pacto pela Saúde.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; O valor do recurso financeiro correspondente a uma parcela extra do incentivo mensal das Equipes de Saúde de Família (ESF) que atuam no PSE. Se faz necessário, portanto, definir o número de Equipes que atuarão no PSE,<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; considerando o território escolar, os analisadores sociais e a cobertura de educandos por ESF, com base no Parâmetro Essencial abaixo:</b>
	     <br><br>
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td style="background: rgb(238, 238, 238)" colspan='4'>
		<center>
		 <span style="font-size: 12"><b>IMPORTANTE</b></span>
		 </center>
		 <br>
	     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 70% DO VALOR TOTAL DO RECURSO FINANCEIRO DO PSE SERÁ PAGO A PARTIR DA ASSINATURA DO TERMO DE COMPROMISSO. OS 30% RESTANTES DO VALOR TOTAL DO RECURSO FINANCEIRO DO PSE SERÁ PAGO APÓS O CUMPRIMENTO DE 70%<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; DAS METAS MUNICIPAIS PACTUADAS.</b>
	     <br><br>
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td style="background: rgb(238, 238, 238)" colspan='4'>
		<center>
		 <span style="font-size: 12"><b>BASE DE CÁLCULO DO PARÂMETRO ESSENCIAL MUNICIPAL</b></span>
		 
		 <br><br><b>
		 COBERTURA ANUAL COMPONENTE I (AVALIAÇÃO CLÍNICA E PSICOSSOCIAL): 500 EDUCANDOS POR ESF
		 <br>
		 COBERTURA ANUAL COMPONENTE II (PROMOÇÃO E PREVENÇÃO À SAÚDE): 1000 EDUCANDOS POR ESF
	     </b>
	     </center>
	     <br>
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td width="50%" class="SubTituloDireita" colspan='2'>Número de Equipes de Saúde da Família do Município:</td>
	<td colspan='2'>
		<?
			if(!$conquantesfmun || $conquantesfmun == 0){
				$sql = "select count(cneid) as total 
						from pse.scnes 
						where muncod = '".$_SESSION['pse']['muncod']."'";
				$conquantesfmun = $db->pegaUm($sql);
			}			
			echo campo_texto( 'conquantesfmun', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
</tr>
<tr>
	<td width="50%" class="SubTituloDireita" colspan='2'>Número de Escolas Públicas:</td>
	<td colspan='2'>
		<? 
			if(!$conquantescolapub || $conquantescolapub == 0){
				$sql = "select count(entid) as total 
						from pse.censopse 
						where muncod = '".$_SESSION['pse']['muncod']."'";
				
				$conquantescolapub = $db->pegaUm($sql);
			}	
			echo campo_texto( 'conquantescolapub', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
</tr>
<tr>
	<td width="50%" class="SubTituloDireita" colspan='2'>Número de Educandos do Município:</td>
	<td colspan='2'>
		<?
			if(!$conquanteducando || $conquanteducando == 0){
				$sql = "select COALESCE(sum(cenquanteducando), 0) as total 
						from pse.censopse 
						where muncod = '".$_SESSION['pse']['muncod']."'";
				$conquanteducando = $db->pegaUm($sql);
			}
			echo campo_texto( 'conquanteducando', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
</tr>
<tr>  
	<td width="50%" class="SubTituloDireita" colspan='2'>Número de Equipes de Saúde da Família que atuarão no PSE:</td>
	<td colspan='2'>
		<?=campo_texto( 'conesfatuarapse', 'S', 'S', '', 20, 6, '######', '', 'right', '', 0, '', 'calcv(this)', '', '');?>
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloDireita">
		Parâmetro Componente I: 
		<?
			if($entid && (!$concoberturacomp1 || $concoberturacomp1 == 0)){
				$sql = "select (conesfatuarapse * 500) as total 
						from pse.contratualizacao
						where entid = $entid";
				$concoberturacomp1 = $db->pegaUm($sql);
			}
			
			echo campo_texto( 'concoberturacomp1', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
	<td class="SubTituloDireita">
		% de Educandos Total Componente I: 
		<?
			if($entid && (!$conpercentualcomp1 || $conpercentualcomp1 == 0)){
				$sql = "select 
							(CASE WHEN conquanteducando > 0 THEN
									(concoberturacomp1 * 100) / conquanteducando 
							      ELSE 
									0 
							END) as total
						from pse.contratualizacao
						where entid = $entid";
				$conpercentualcomp1 = $db->pegaUm($sql);
				if($conpercentualcomp1) $conpercentualcomp1 = number_format($conpercentualcomp1,2,",",".");
			}
			
			echo campo_texto( 'conpercentualcomp1', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
	<td class="SubTituloDireita">
		Parâmetro Componente II: 
		<?
			if($entid && (!$concoberturacomp2 || $concoberturacomp2 == 0)){
				$sql = "select (conesfatuarapse * 1000) as total 
						from pse.contratualizacao
						where entid = $entid";
				$concoberturacomp2 = $db->pegaUm($sql);
			}
			//ver($sql);
			echo campo_texto( 'concoberturacomp2', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
	<td class="SubTituloDireita">
		% de Educandos Total Componente II: 
		<?
			if($entid && (!$conpercentualcomp2 || $conpercentualcomp2 == 0)){
				$sql = "select (CASE WHEN conquanteducando > 0 THEN
									(concoberturacomp2 * 100) / conquanteducando 
							      ELSE 
									0 
							END) as total 
						from pse.contratualizacao
						where entid = $entid";
				$conpercentualcomp2 = $db->pegaUm($sql);
				if($conpercentualcomp2) $conpercentualcomp2 = number_format($conpercentualcomp2,2,",",".");
			}
			
			echo campo_texto( 'conpercentualcomp2', 'N', 'N', '', 20, 6, '', '', 'right', '', 0, '');
		?>
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan='4'>&nbsp;</td>
</tr>
<tr>
	<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
		<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaForm();">
	</td>
</tr>
</table>


</form>

<script type="text/javascript">

var d = document.formulario;

function validaForm(){

	if(d.conesfatuarapse.value == ''){
		alert ('Favor Preencher o campo obrigatório: Número de Equipes de Saúde da Família que atuarão no PSE.');
		d.conesfatuarapse.focus();
		return false;
	}
	
	d.submit();

}

function calcv(obj){

	
	if( parseFloat(obj.value) > parseFloat(d.conquantesfmun.value) ){
		alert("Número de Equipes de Saúde da Família que atuarão no PSE\nnão pode ser maior que\nNúmero de Equipes de Saúde da Família do Município.");
		obj.value = d.conquantesfmun.value;
		d.conquantesfmun.focus;
	}

	if( (obj.value * 500) > d.conquanteducando.value ){
		d.concoberturacomp1.value = d.conquanteducando.value;
	} else {
		d.concoberturacomp1.value = obj.value * 500;
	}

	if( (obj.value * 1000) > d.conquanteducando.value ){
		d.concoberturacomp2.value = d.conquanteducando.value;
	} else {
		d.concoberturacomp2.value = obj.value * 1000;
	}

	if(d.conquanteducando.value > 0){
		d.conpercentualcomp1.value = (d.concoberturacomp1.value * 100) / d.conquanteducando.value;
		d.conpercentualcomp2.value = (d.concoberturacomp2.value * 100) / d.conquanteducando.value;
	}
	else{
		d.conpercentualcomp1.value = 0;
		d.conpercentualcomp2.value = 0;
	}

	d.conpercentualcomp1.value = float2moeda(d.conpercentualcomp1.value);
	d.conpercentualcomp2.value = float2moeda(d.conpercentualcomp2.value);
	
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

<?php if($_SESSION['permissaoedicao'] == false){?>
	var obj = document.getElementsByTagName("input");
	var total = document.getElementsByTagName("input").length;
	
	for(i=0; i<total; i++){
		obj[i].disabled = true;
	}
<?php }?>

</script>
