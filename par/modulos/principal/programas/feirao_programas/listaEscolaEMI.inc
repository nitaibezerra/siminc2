<?php

if(!$_SESSION['par']['adpid'] || !$_SESSION['par']['prgid']){
	echo '<script type="text/javascript"> 
    		alert("Sessão Expirou.\nFavor selecione o programa novamente!");
    		window.location.href="par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa";
    	  </script>';
    die;
}


if($_POST){
	$sql = "delete from par.pfemiescolas where adpid = ".$_SESSION['par']['adpid'];
	$db->executar($sql);
	
	if($_POST['entid'][0]){
		foreach($_POST['entid'] as $entid){
			$sql = "INSERT INTO par.pfemiescolas(entcodent, adpid)
    				VALUES (".$entid.", ".$_SESSION['par']['adpid'].")";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	echo '<script type="text/javascript"> 
    		alert("Operação Efetuada com Sucesso!");
    		location.href="par.php?modulo=principal/programas/feirao_programas/listaEscolaEMI&acao=A";
    	  </script>';
    die;
}

	
include APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$sql = "select prgdsc from par.programa where prgid = ".$_SESSION['par']['prgid'];
$nomePrograma = $db->pegaUm($sql);

if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$descricao = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$descricao = $_SESSION['par']['estuf'];
}
	
monta_titulo($nomePrograma,$descricao);
echo '<br/>';
	
echo carregaAbasProFuncionario('par.php?modulo=principal/programas/feirao_programas/listaEscolaEMI&acao=A');
monta_titulo( 'Escolas', '' );


//permissao edição
$programa = pegarProgramaDisponivel();
$programa = $programa ? $programa : array();
$habilitado = 'S';
if( !in_array(PROGRAMA_EMI, $programa) ){
	$habilitado = 'N';
}
?>
<body>
<form name="formulario" method="post">

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	  <tr>
		<td width="20%" align='right' class="SubTituloDireita">Informe a(s) Escola(s):</td>
		<td>
			<?php
			/*
			if($_SESSION['par']['itrid'] == 2){
				$stWhere = "AND mun.pk_cod_municipio = '{$_SESSION['par']['muncod']}'";
			}else{
				$stWhere = "AND uf.sigla = '{$_SESSION['par']['estuf']}'";
			}
			*/
			$stWhere = "AND edr.estuf = '{$_SESSION['par']['estuf']}'";
			
			$sql_combo = "SELECT 
								ent.entcodent as codigo, 
								'- ' || mun.estuf ||'/'|| replace(mun.mundescricao,chr(39),'') ||': '|| ent.entcodent ||' - '|| replace(ent.entnome,chr(39),'') as descricao
							FROM entidade.entidade ent
							inner join par.pfeemicenso pm on pm.inep = ent.entcodent and pm.pecanoreferencia = 2012
							inner join entidade.endereco edr on edr.entid = ent.entid
							inner join territorios.municipio mun on mun.muncod = edr.muncod
							where ent.entstatus = 'A'
							$stWhere 
							order by 2";
			
			if ($_SESSION['par']['adpid']){					
				$sql = "SELECT 
								ent.entcodent as codigo, 
								'- ' || mun.estuf ||'/'|| replace(mun.mundescricao,chr(39),'') ||': '|| ent.entcodent ||' - '|| replace(ent.entnome,chr(39),'') as descricao
							FROM entidade.entidade ent
							inner join par.pfeemicenso pm on pm.inep = ent.entcodent and pm.pecanoreferencia = 2012
							inner join entidade.endereco edr on edr.entid = ent.entid
							inner join territorios.municipio mun on mun.muncod = edr.muncod
							inner join par.pfemiescolas pf on pf.entcodent = pm.inep
							where pf.adpid = '{$_SESSION['par']['adpid']}'
							$stWhere
							group by 1,2
							order by 2";
				//dbg($sql);
				$nome = 'entid';
				$$nome = $db->carregar( $sql ); 
			}	
			$where = array(
								array(
										"codigo" 	=> "ent.entcodent",
										"descricao" => "Código INEP:",
										"numeric"	=> false
							   		  ),
							    array(
										"codigo" 	=> "mun.mundescricao",
										"descricao" => "Município:",
										"numeric"	=> false
							   		  ),
							    array(
										"codigo" 	=> "ent.entnome",
										"descricao" => "Nome da Escola:",
										"numeric"	=> false
							   		  )
							   );
			combo_popup( 'entid', $sql_combo, 'Selecione a(s) Escola(s)', '360x460', '', '', '', $habilitado, true, '', '15', '800', '', '', false, $where, '', $mostraPesquisa = true, $campo_busca_descricao = true, '', false, '' , '' );
			?>
		</td>
		<td align="center" >
		
			<?
				include_once APPRAIZ . "includes/workflow.php";
				$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
				wf_desenhaBarraNavegacao(pgCriarDocumento( $_SESSION['par']['adpid'] ), $dados_wf);
			?>
		</td>
	  </tr> 
	  <tr>
		<td class="SubTituloCentro" colspan="2">
			<br>
			<?if($habilitado == 'S'){?>
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvar();">
			<?}?>
		</td>
	 <tr>
</table>
	
</form>
</body>

<script type="text/javascript">


function salvar()
{
	selectAllOptions( document.getElementById( 'entid' ) );
	document.formulario.submit();
}

</script>

	