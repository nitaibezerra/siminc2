<?php 
				
$_SESSION['par']['cod_inep'] = $_REQUEST['cod_inep'] ? $_REQUEST['cod_inep'] : $_SESSION['par']['cod_inep'];

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";

/*MONTA TITULO*/
monta_titulo('FINANCIAMENTO DE NOVAS TURMAS DE EJA', '');
echo "<br>";
$sql = "SELECT DISTINCT
			no_entidade as escola
		FROM 
			".SCHEMAEDUCACENSO.".tab_entidade
		WHERE
			cod_orgao_regional_inep = '".$_REQUEST['cod_inep']."'";
$linha1 = $db->pegaUm($sql); 
if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$linha2 = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$linha2 = $_SESSION['par']['estuf'];
}
monta_titulo($linha1, $linha2);

echo "<br/>"; 
$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo('Termo de Adesão',''); 
/*FIM MONTA TITULO*/
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
	});
</script>
<center>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td width="90%" align="center">
				<br />
				<div style="overflow: auto; height:400px; width: 70%; background-color: rgb(250,250,250); border-width:1px; border-style:solid;" align="left" >
					<div style="width: 95%; magin-top: 10px; padding:10px;">
					
					<center>
					<img width="80" height="80" src="/imagens/brasao.gif" />
					<h2>
					<p>MINISTÉRIO DA EDUCAÇÃO<br/>
					TERMO DE ADESÃO</p>
					</h2>
					</center>
					<br>
					<center>
						<div style="text-align:justify;float:center;width:70%">
							<font size="2">
								<p>
								O Município de <b><?=$linha2 ?></b> resolve firmar o presente
								Termo de Adesão ao Programa de Manutenção de Novas Turmas de EJA em
								conformidade com o estabelecido na Resolução CD/FNDE nº XX/2012, que
								aprova os procedimentos para a implantação de novas turmas de Educação
								de Jovens e Adultos, nas modalidades presencial e integrada à
								qualificação profissional, por meio da transferência antecipada de
								recursos financeiros do Fundo de Manutenção e Desenvolvimento da
								Educação Básica (Fundeb), de que trata a Lei nº 11.494 de 20 de junho
								de 2007.
								</p>
							</font>
						</div>
					</center>
					</div>
				</div>
			</td>
		</tr>
	</table>
	<p>
	<input type="button" value="Próximo" onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/planoAcoes&acao=A'" />
	<input type="button" value="Sair" 	 onclick="window.location.href = 'par.php?modulo=principal/programas/fin_nov_tur_eja/lista_escolas&acao=A'" />
</center>
