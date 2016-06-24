<?php 
				
define('ESCOLADATERRA_ADERIRAM', 				1);
define('ESCOLADATERRA_NAOADERIRAM', 			2);
define('ESCOLADATERRA_CLASSESMULTISERIADAS', 	3);				

if($_REQUEST['requisicao'] == 'adesaoPrograma'){
	
	$sql = "update par.escolaativa set 
				usucpfescolaativa = '{$_SESSION['usucpf']}',
				esasituacaoadesao = ".$_REQUEST['esasituacaoadesao']."
			where 
				inuid = {$_SESSION['par']['inuid']} ";
	
	$db->executar($sql);
	if($db->commit()){
		echo $_REQUEST['esasituacaoadesao'];
		die;
	}
	echo 0;
}

if($_SESSION['par']['inuid']){
	
	$sql = "select
				esaid, 
				esasituacaoadesao 
			from 
				par.escolaativa 
			where 
				inuid = {$_SESSION['par']['inuid']}";
	
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);	
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";



$linha1 = 'Escola da Terra'; 
if($_SESSION['par']['muncod']){
	
	$sql = "select mundescricao, estuf from territorios.municipio where muncod = '{$_SESSION['par']['muncod']}'";
	$rsMunicipio = $db->pegaLinha($sql);
	$linha2 = $rsMunicipio['mundescricao'].' - '.$rsMunicipio['estuf'];
	
}else{
	
	$linha2 = $_SESSION['par']['estuf'];
}
monta_titulo($linha1, $linha2);

if(isset($esasituacaoadesao) && $esasituacaoadesao == 1){
	echo "<script>
			window.location.href = 'par.php?modulo=principal/programas/escolaativa/listaEscolas&acao=A';
		  </script>";
}else
if(isset($esasituacaoadesao) && (int) $esasituacaoadesao == 2){
	$stResposta =  "Não confirmou a adesão.";	
} else
if (isset($esasituacaoadesao) && $esasituacaoadesao == 3){
	$stResposta = "Não há classes multisseriadas.";	
} else 
if(!isset($esaid)){
	$stResposta = "O município não possui escola ativa.";	
}

?>
<?php if(!isset($esasituacaoadesao)): ?>
	<?php echo "<br/>"; monta_titulo('Termo de Adesão',''); ?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script>
		$(function(){
			$('.adesaoEscolaTerra').click(function(){
	
				$.ajax({
					url		: 'par.php?modulo=principal/programas/escolaativa/termoadesao&acao=A',
					type	: 'post',
					data	: 'requisicao=adesaoPrograma&esasituacaoadesao='+this.id,
					success	: function(e){
						if(e == 1){
							window.location.href = 'par.php?modulo=principal/programas/escolaativa/listaEscolas&acao=A';						
						}else
						if(e == 2 || e == 3){
							window.location.href = 'par.php?modulo=principal/programas/escolaativa/termoadesao&acao=A';
						}else{
							alert('Erro ao realizar operação!');
							window.location.href = 'par.php?modulo=principal/programas/escolaativa/termoadesao&acao=A';
						}						
					}
				});
			});
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
						SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO  <br/>DIVERSIDADE E INCLUSÃO<br/>
						DIRETORIA DE POLÍTICAS DE EDUCAÇÃO DO CAMPO, <br/>INDÍGENA E PARA AS RELAÇÕES ÉTNICO-RACIAIS.</p>
						</h2>
						</center>
						
						<font size="2">
							<p>Senhor (a) Secretário (a) de Educação,</p>
							
							<p>O Ministério da Educação, no âmbito das ações do Plano de Desenvolvimento da Educação, lança o Programa Nacional de Educação do Campo, que estabelece 
							dentre seus atos o Programa Escola da Terra, para beneficiar estados e municípios de todas as regiões do País. O Programa destina-se às escolas do 
							campo que oferecem os anos iniciais do ensino fundamental, nos sistemas municipal e estadual, em turmas organizadas sob a forma de multisseriação.<br/> 
							Para o ano em curso, será feita a adesão dos estados e municípios que não foram atendidos pelo Programa "Escola Ativa".</p>
							
							<p>Para fazer a adesão, o (a) secretário (a) de educação deverá acessar o Plano de Ações Articuladas do estado/município no Simec, pelo endereço eletrônico 
							<a href="http://simec.mec.gov.br" target="_blank">http://simec.mec.gov.br</a>, onde está disponível o Termo de Adesão. Para orientar o processo de adesão, encaminha-se um arquivo com os procedimentos a 
							serem adotados (Passo a Passo da Adesão 2012 - Programa Escola da Terra).</p>
							 
							<p>Ressalta-se que a adesão pode ser realizada pelos dirigentes que têm escolas com classes multisseriadas na sua rede de ensino e tem interesse em
							implementar o Programa Escola da Terra; para aqueles que já aderiram ao Programa Escola Ativa em anos anteriores e desejam expandir para outras 
							escolas multisseriadas da rede, que porventura ainda não foram atendidas, o processo poderá ser realizado a partir de 2013.</p>
							
							<p>Colocamo-nos à disposição para quaisquer esclarecimentos que se fizerem necessários.</p>					
							
							<p>Atenciosamente,<br/>
							Coordenação Geral de Políticas de Educação do Campo/SECADI/MEC.<br/>
							<a href="mailto:<?php echo $_SESSION['email_sistema']; ?>"><?php echo $_SESSION['email_sistema']; ?></a></p>
						</font>
						</div>
					</div>
				</td>
			</tr>
		</table>
		<p>
		<input type="button" name="aceito" value="Iniciar Processo de Adesão" class="adesaoEscolaTerra" id="1"/>
		<input type="button" name="naceito" value="Não Confirmo a Adesão" class="adesaoEscolaTerra" id="2" />
		<br/>
		<input type="button" name="naohaclassesmulti" value="Não há Classes Multisseriadas" class="adesaoEscolaTerra" id="3" />
		<input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
	</center>
<?php else: ?>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td align="center" valign="middle" height="40">				
				<font size="2"><?php echo $stResposta; ?></font>
			</td>
		</tr>
	</table>
	<center>
		<p><input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" /></p>
	</center>
<?php endif; ?>
