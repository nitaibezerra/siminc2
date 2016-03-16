<?php

function montaAndarDemanda($lcaid){
	global $db;
 
	$sql = sprintf("SELECT 
					 laa.laaid AS codigo,
					 aa.anddescricao AS descricao
					 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
					FROM 
					 	demandas.localandaratendimento laa
						inner join demandas.andaratendimento aa on laa.andid = aa.andid
					WHERE
					 laa.lcaid = %d 
					ORDER BY 
					 aa.anddescricao", 
			$lcaid);
	$dados = $db->carregar($sql);
	//echo 'ok='.count($dados).'<br>';
	if(count($dados)==1) $laaid = $dados[0]['codigo'];
	//echo $laaid;
	$db->monta_combo( 'laaid', $sql, 'S', '-- Informe o Andar --', '', '', '', '', '', '', '', $laaid );
	echo obrigatorio();
}

//filtra o andar
if ($_REQUEST['lcaidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaAndarDemanda($_POST['lcaidAjax']);
	exit;
}


if ($_REQUEST['salvar']){

	$usdcelular = str_replace("-","",$_POST['usdcelular']);
 	if(!$_POST['usdgabinete']) $_POST['usdgabinete'] = 'f';
 	$_POST['ccoid'] = $_POST['ccoid'] ? $_POST['ccoid'] : 'null';
 	$_POST['unaid'] = $_POST['unaid'] ? $_POST['unaid'] : 'null';
 	$_POST['laaid'] = $_POST['laaid'] ? $_POST['laaid'] : 'null';
 	 	
	$sql = " UPDATE demandas.usuariodetalhes SET 
						usdgabinete = '".$_POST['usdgabinete']."',
						ccoid = ".$_POST['ccoid'].",
						usdramal = '".$_POST['usdramal']."',
						usdcelular = '".$usdcelular."',
						unaid = ".$_POST['unaid'].",
						laaid = ".$_POST['laaid'].",
						usdsala = '".$_POST['dmdsalaatendimento']."',
						usdnomecomputador = '".$_POST['usdnomecomputador']."'
					 WHERE usucpf = '".$_POST['cpf']."'";
	$db->executar($sql);
	$db->commit();
	
	die("<script>
			//window.opener.location.reload();
			alert('Operação realizada com sucesso!');
			window.close();
		 </script>");
	 	
}


$sql = "select 
			u.usucpf, u.usunome, ud.ccoid, ud.unaid, ud.laaid, 
			l.lcaid, ud.usdsala as dmdsalaatendimento, ud.usdgabinete, ud.usdramal, 
       		ud.usdcelular, ud.usdnomecomputador 
		from demandas.usuariodetalhes ud 
		inner join seguranca.usuario u ON u.usucpf=ud.usucpf
		left join demandas.localandaratendimento l ON l.laaid = ud.laaid
		where u.usucpf = '".$_REQUEST['cpf']."'";

$dados = $db->pegaLinha($sql);

if($dados) {
	extract($dados);
}
else{
	$sql = "INSERT INTO demandas.usuariodetalhes(usucpf) VALUES ('".$_REQUEST['cpf']."')";
	$db->executar($sql);
	$db->commit();
}






?>
<html>
<body style="font-size:7pt;" leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">

<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>

<form name="formulario" action="" method="post">

<input type="hidden" name="cpf" value="<?=$_REQUEST['cpf']?>">
<input type="hidden" name="salvar" value="">

<table width="99%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td style="background-color: #d0d0d0; text-align: center;" colspan="2">
			<b>EDITAR DETALHES SOLICITANTE</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Solicitante:</td>
		<td>
			<?=$usunome?>
		</td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita">Cargo DAS:</td>
		<td>
		<?php
			
			$sql = "SELECT 
					 ccoid AS codigo,
					 ccodsc AS descricao
					FROM 
					 demandas.cargocomissionado
					WHERE
					ccostatus = 'A'
					order by 1
					";
			$db->monta_combo( 'ccoid', $sql, 'S', '-- Selecione o Cargo --', '', '' );
			
			
		?>	
		</td>
	</tr>	
	<tr>
		<td class="subtitulodireita">Setor:</td>
		<td>
			<?php 
				$sql = " SELECT
						  unaid AS codigo, 
					  	  upper(unasigla)||' - '||unadescricao as descricao 
						 FROM
						  demandas.unidadeatendimento 
						 WHERE
						  unastatus = 'A'
						  order by unasigla, unadescricao";
				$db->monta_combo("unaid",$sql,'S',"-- Selecione um setor --",'','', '', '', '', 'unaid', '');
			?>
			
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Edifício:</td>
		<td>
		<?php
		$sql = "SELECT 
				 lcaid AS codigo,
				 lcadescricao AS descricao
				FROM 
				 demandas.localatendimento
				ORDER BY 
				 lcadescricao";
		$db->monta_combo( 'lcaid', $sql, 'S', '-- Informe o local de atendimento --', 'filtraAndar', '', '', '', '', 'lcaid', '' );
		?>	
		
		</td>
	</tr>	
	<tr >
		<td align='right' class="SubTituloDireita" >Andar:</td>
		<td id='andardemanda'>
			<?
			if(!$lcaid) $lcaid = '999999';
			if($lcaid){
				$sql = sprintf("SELECT 
								 laa.laaid AS codigo,
								 aa.anddescricao AS descricao
								 --laa.laaid, laa.lcaid, aa.andid, aa.anddescricao 
								FROM 
								 	demandas.localandaratendimento laa
									inner join demandas.andaratendimento aa on laa.andid = aa.andid
								WHERE
								 laa.lcaid = %d 
								ORDER BY 
								 aa.anddescricao", 
						$lcaid);
				$db->monta_combo( 'laaid', $sql, 'S', '-- Informe o Andar --', '', '', '', '', '', 'laaid', '', $laaid );
				echo obrigatorio();
			}
			?>
		</td>
	</tr>				
	<tr >
		<td align='right' class="SubTituloDireita">Sala:</td>
		<td><?= campo_texto( 'dmdsalaatendimento', 'N', 'S', '', 20, 50, '', '', '', '', '', 'id="dmdsalaatendimento"'); ?></td>
	</tr>		
	<tr>
		<td align='right' class="SubTituloDireita">Sala fica no Gabinete:</td>
		<td>
			<input type="radio" name="usdgabinete" id="usdgabinete" value="t" <?if($usdgabinete == 't') echo "checked";?>> Sim
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<input type="radio" name="usdgabinete" id="usdgabinete" value="f" <?if($usdgabinete == 'f' || $usdgabinete == '') echo "checked";?>> Não
		</td>
	</tr>				
	<tr>
		<td align='right' class="SubTituloDireita">Ramal:</td>
		<td><?= campo_texto( 'usdramal', 'N', 'S', '', 4, 4, '####', '', '', '', '', 'id="usdramal"'); ?></td>
	</tr>		
	<tr>
		<td align='right' class="SubTituloDireita">Celular:</td>
		<td><?= campo_texto( 'usdcelular', 'N', 'S', '', 10, 9, '####-####', '', '', '', '', 'id="usdcelular"'); ?></td>
	</tr>	
	<tr>
		<td  align='right' class="SubTituloDireita">Nome do Computador(CPU):<br>Clique com o botão direito do mouse no atalho do "Meu computador", depois clique em "Propriedades", depois clique na aba "Nome do computador". Ex.: mec-32-2014.mec.gov.br (Digite somente mec-32-2014).</td>
		<td><?= campo_texto( 'usdnomecomputador', 'N', 'S', '', 40, 100, '', ''); ?></td>
	</tr>
		
	<tr bgcolor="#C0C0C0">
		<td colspan="2" align="center">
			<input type="button" class="botao" name="btalterar" id="btalterar" value="Salvar" onclick="validaForm();">
		</td>
	</tr>	
</table>
</form>
</body>
</html>

<script type="text/javascript">
/*
* Function destinada a validação do formulário
*/
function validaForm(){
	d = document.formulario;
	
	if (d.lcaid.value != ''){
		if (d.laaid.value == ''){
			alert('O campo Andar é obrigatório!');
			d.laaid.focus();
			return false;
		}
	}
	d.btalterar.disabled = true;
	d.salvar.value='1';
	d.submit();
}



/*
* Faz requisição via ajax
* Filtra o andar, atravéz do parametro passado 'lcaid'
*/
function filtraAndar(lcaid) {
	d = document;
	
	if(!lcaid) lcaid = '999999';

	td     = d.getElementById('andardemanda');	
	select = td.getElementsByTagName('select')[0];
	
	// Desabilita o <select>, caso exista, até que o resultado da pesquisa seja carregado
	if (select)
		select.disabled = true;
		
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('demandas.php?modulo=principal/popUsuarioDetalhes&acao=A', {
							        method:     'post',
							        parameters: '&lcaidAjax=' + lcaid,
							        onComplete: function (res)
							        {			
										td.innerHTML = res.responseText;
							        }
							  });
}

</script>
