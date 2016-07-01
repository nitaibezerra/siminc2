<?php 

$_SESSION['par']['cod_inep'] = $_REQUEST['cod_inep'] ? $_REQUEST['cod_inep'] : $_SESSION['par']['cod_inep'];

function pegarDocidEJA( $cod_inep ) {

	global $db;

	require_once APPRAIZ . 'includes/workflow.php';
	
	$sql = "SELECT
				docid
			FROM
				par.instrumentoeja 
			WHERE
				pk_cod_entidade = ".$cod_inep;
	
	$docid = $db->pegaUm( $sql );

	if( !$docid ) {
		
		$tpdid = 83;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza pap do EMI
		$sql = "UPDATE
					par.instrumentoeja 
				SET
					docid = {$docid}
				WHERE
					pk_cod_entidade = {$cod_inep}";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;

}

function adesaoPrograma( $request ){
	
	global $db;
	
	$docid = pegarDocidEJA( $_SESSION['par']['cod_inep'] );
	
	$request['iejobs'] = $request['iejobs'] ? "'".$request['iejobs']."'" : 'null';
	
	$sql = "INSERT INTO par.instrumentoeja(docid, pk_cod_entidade, iejtermo, iejobs, iejdtinclusao)
  			VALUES ( $docid, ".$_SESSION['par']['cod_inep'].", ".$request['iejtermo'].", ".$request['iejobs'].", now());";
	
	$db->executar($sql);
	if($db->commit()){
		echo $request['iejtermo'];
		die;
	}
	echo 0;
}

if( $_REQUEST['reqAjax'] ){
	$_REQUEST['reqAjax']($_REQUEST);
	die();	
}

if($_SESSION['par']['cod_inep']){
	
	$sql = "SELECT
				iejtermo,
				iejobs
			FROM
				par.instrumentoeja
			WHERE
				pk_cod_entidade = {$_SESSION['par']['cod_inep']}";
	
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);	
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
echo "<br/>";


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

echo "<br/>"; monta_titulo('Termo de Adesão',''); 
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('.adesaoEscola').click(function(){
			var iejobs = $('#iejobs').val();
			$.ajax({
				url		: window.location,
				type	: 'post',
				data	: 'reqAjax=adesaoPrograma&iejtermo='+this.id+'&iejobs='+iejobs,
				success	: function(e){
					if( e == 'true' ){
						window.location = 'par.php?modulo=principal/programas/fin_nov_tur_eja/formularioAdesao&acao=A';						
					}else
					if( e == 'false' ){
						window.location.href = window.location;
					}else{
						alert('Erro ao realizar operação!');
						window.location.href = window.location
					}						
				}
			});
		});
		
		$('.escondeAdesaoEscola').click(function(){
			$('#just-container').hide();
		});
		$('.mostraAdesaoEscola').click(function(){
			$('#just-container').show();
		});
<?php 
if( $iejtermo == 't' ){
?>
		window.location = 'par.php?modulo=principal/programas/fin_nov_tur_eja/formularioAdesao&acao=A';
<?php 
}
?>
	});
</script>
<center>
<?php 
if( $iejtermo == 'f' ){
?>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td width="90%" align="center">
				Recusou.<br>
				<b>Justificativa:</b><?=$iejobs ?>
			</td>
		</tr>
	</table>
</center>
<?php 
}else{
?>
	<div id="just-container" style="position: absolute; width:100%; height:2000%; display:none; ">
		<div style="position: absolute; background-color: black; opacity: .6; width:100%; height:2000%" >
		</div>
		<div style="position: absolute; float:left; margin-left:445px; margin-top:100px; width:30%; height:150px; opacity: 1; background-color: white; " align="left">	
			<div style="float:right;cursor:pointer;" class="escondeAdesaoEscola">
				<img border="0" title="Cancela." width="18px" src="../imagens/excluir_2.gif">
			</div>
			<br>
			<div style="margin-left:15px;">
				<b>Justifique:</b><br>				
				<?=campo_textarea('iejobs', 'S', 'S', '', 50, 5, 255)?>
			</div>
			<div  style="float:right; margin-right:15px;">
				<input type="button" name="naceito"  value="Não Confirmo a Adesão" 		class="adesaoEscola" id="false" />
			</div>
		</div>
	</div>
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
	<input type="button" name="aceito"   value="Iniciar Processo de Adesão" class="adesaoEscola" id="true"/>
	<input type="button" name="naceito"  value="Não Confirmo a Adesão" 		class="mostraAdesaoEscola" id="false" />
	<input type="button" name="cancelar" value="Sair" onclick="window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa'" />
</center>
<?php 
}
?>
