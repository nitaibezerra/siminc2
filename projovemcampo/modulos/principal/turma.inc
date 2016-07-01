<?
if(!$_SESSION['projovemcampo']['apcid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");
$perfis = pegaPerfilGeral();

$sql = "SELECT DISTINCT
			esdid
		FROM
			projovemcampo.adesaoprojovemcampo apc
		INNER JOIN workflow.documento doc ON doc.docid = apc.docid
		WHERE
			apcid = {$_SESSION['projovemcampo']['apcid']}";

$estadodocumento = $db->pegaUm( $sql );
$disable = '';
$bloq = 'S';
if($estadodocumento!=ESD_EMELABORACAO){
	$disable = "disabled='disabled'";
	$bloq = 'N';
}
$meta = pegameta();

if($_REQUEST['turid']){
	$turma3 = "AND turid != {$_REQUEST['turid']}";
}
$sqlqtdturmas = "
				SELECT 
					count(turid) as qtdturmas,
					SUM(turqtdalunosprevistos) as previsto
				FROM  
						projovemcampo.turma t
				INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = t.secaid
				WHERE
					a.apcid = {$_SESSION['projovemcampo'][apcid]}
				AND turstatus = 'A'
				$turma3
				";
$qtdturmas = $db->pegaLinha( $sqlqtdturmas );

if(!$qtdturmas['qtdturmas']){
	$qtdturmas['qtdturmas'] = 0;
	$qtdturmas['previsto'] = 0;
}

if($_SESSION['projovemcampo']['muncod']){
	$sql = "SELECT 
				estuf as estuf,
				mundescricao as municipio	
			FROM 
				territorios.municipio 
			WHERE 
				muncod = '{$_SESSION['projovemcampo']['muncod']}'";
	$dadosmun = $db->pegaLinha( $sql );
}
if($_REQUEST['turid']){
	$sqldadosturma = "
					SELECT
						entcodent,
						turdescricao,
						turqtdalunosprevistos
					FROM
							entidade.entidade ent
					INNER JOIN	projovemcampo.turma t ON t.entid = ent.entid
					WHERE
						turid ={$_REQUEST['turid']}";
		    
	$dadosturma = $db->pegaLinha($sqldadosturma);
	$entcodent = $dadosturma['entcodent'];	
	$turqtdalunosprevistos = $dadosturma['turqtdalunosprevistos'];
	    
}
?>
<html>
	<head>
	<script language="javascript" type="text/javascript" src="./js/projovemcampo.js"></script>
  	<script language="JavaScript" src="../includes/funcoes.js"></script>
  	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
   	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
   	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
   	<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
   	<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
   	<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
   	<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
  
   	</head>
   	<body>
   	<script>
   	jQuery(document).ready(function() {
		if( '<?=$entcodent ?>' != '' ){
			jQuery('#entcodent').blur();
		}
	});
   	
   	function gravarTurma() {
   		var turid	 	 = jQuery('#turid').val();
		if(jQuery('#entcodent').val()=='') {
			alert('Preencha o código INEP');
			return false;
		}
	    		
		if(jQuery('#turqtdalunosprevistos').val()=='') {
			alert('Preencha a quantidade de alunos');
			return false;
		}
		if(15 > jQuery('#turqtdalunosprevistos').val()|| jQuery('#turqtdalunosprevistos').val()>30){
				jQuery('#turqtdalunosprevistos').val('');
			alert('A quantidade de alunos por turma deve ser de no mínimo 15 e de no máximo 30 alunos.');
			return false;
		}
		var totalinscri  = parseInt(<?=$qtdturmas['previsto']?>);
		var qtdturmas	 = parseInt(<?=$qtdturmas['qtdturmas']?>);
		var meta	 	 = parseInt(<?=$meta?>);
		var turmas1 	 = 1;

		var totalfinalalunos = (parseFloat(totalinscri) + parseFloat(jQuery('#turqtdalunosprevistos').val()));
		
		var diferenca    = (parseFloat(totalfinalalunos) - parseFloat(meta));
		
		var total		 = (parseFloat(turmas1) + parseFloat(qtdturmas));
			
		var maxqtd   	 = (meta/15);
	    	
		maxqtd = maxqtd.toFixed(0);
		
		if(totalfinalalunos > meta){
			alert('Você ultrapassou a meta em '+diferenca+'.');
			return false;
		}
		if( maxqtd < 1 ) {
			alert('Para que se possa cadastrar uma turma é necessário ter ao menos 15 alunos.');
			return false;
		}
		if(turid ==''){	
 
			if( total>maxqtd ) {
				alert('A quantidade total de turmas não deve ser maior que '+maxqtd+'.');
				return false;
			}
   		}
		$('#form').submit();
	}
    	
	function buscarEscolaPorINEP(codinep, escolatipo) {
		if(codinep=='') {
			alert('Digite um código INEP');
			return false;
		}
		var outroTipo = '';
		if( escolatipo == '' ){ outrotipo = '2' }else{ outrotipo = '' }
			jQuery.ajax({
			type: "POST",
			url: "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A",
			data: "requisicao=buscarEscolaPorINEP&codinep="+codinep,
			async: false,
			success: function(msg){
				var myObject = eval('(' + msg + ')');
				if(myObject.entid) {
					if( myObject.entid == $('#entid'+outrotipo).val()  ){
						alert('Escola repetida. Escolha outra.');
						$('#entcodent'+escolatipo).val('');
						$('#entid'+escolatipo).val('');
						$('#td_entnome'+escolatipo).html('');
						$('#td_tpcdesc'+escolatipo).html('');
						$('#td_tpldesc'+escolatipo).html('');
       	   					
						$('#td_endlog'+escolatipo).html('');
						$('#td_endnum'+escolatipo).html('');
						$('#td_endcom'+escolatipo).html('');
						$('#td_endbai'+escolatipo).html('');
						$('#td_endcep'+escolatipo).html('');
						$('#td_mundescricao'+escolatipo).html('');
						$('#td_estuf'+escolatipo).html('');
						$('#td_enttelefone'+escolatipo).html('');
						return false;
					}
       					$('#entid'+escolatipo).val(myObject.entid);
       					$('#td_entnome'+escolatipo).html(myObject.entnome);
       					$('#td_tpcdesc'+escolatipo).html(myObject.tpcdesc);
       					$('#td_tpldesc'+escolatipo).html(myObject.tpldesc);
       					
       					$('#td_endlog'+escolatipo).html(myObject['endereco'].endlog);
       					$('#td_endnum'+escolatipo).html(myObject['endereco'].endnum);
       					$('#td_endcom'+escolatipo).html(myObject['endereco'].endcom);
       					$('#td_endbai'+escolatipo).html(myObject['endereco'].endbai);
       					$('#td_endcep'+escolatipo).html(myObject['endereco'].endcep);
       					$('#td_mundescricao'+escolatipo).html(myObject['endereco'].mundescricao);
       					$('#td_estuf'+escolatipo).html(myObject['endereco'].estuf);
       					$('#td_enttelefone'+escolatipo).html('('+myObject.entnumdddcomercial+')'+myObject.entnumcomercial);
       				} else {
       					alert('Escola não identificada');
       					jQuery('#entcodent'+escolatipo).val('');
       				}
    	   		}
    	 	});
    	}
		function editarTurma(turid) {
			window.location.href = 'projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma&turid='+turid;
		}
		function detalharEndereco(entid) {
		    window.open('projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&requisicao=enderecoEscola&entid='+entid, 'Detalhamento do Endereço', 'scrollbars=yes,height=250,width=600,status=no,toolbar=no,menubar=no,location=no')
		}
    	</script>
    	<form id="form" name="form" method="POST">
    	<input type="hidden" name="requisicao" value="gravarTurma">
    	<input type="hidden" name="meta" id="meta" value="<?php echo $meta;?>">
    	<input type="hidden" name="qtdturmas" id="qtdturmas" value="<?php echo $qtdturmas;?>">
    	<input type="hidden" name="entid" id="entid" value="">
    	<input type="hidden" name="entid2" id="entid2" value="">
    	<input type="hidden" name="turid" id="turid" value="<?=$_REQUEST['turid'] ?>">
    	<input type="hidden" name="mnuid" id="mnuid" value="<?=$_REQUEST['mnuid'] ?>">
    	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    		<tr>
				<td class="SubTituloDireita" width="30%">Orientações</td>
				<td>
					<font color=blue>
					- A quantidade de alunos da turma deve ser de no mínimo 15 e no máximo 30 alunos..
					</font>
				</td>
			</tr>
    		<tr>
    			<td class="SubTituloCentro" colspan="2">Escola</td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita" width="30%">UF:</td>
    			<?if($_SESSION['projovemcampo']['estuf']){?>
    				<td><?=$_SESSION['projovemcampo']['estuf']?></td>
    			<?}elseif($_SESSION['projovemcampo']['muncod']){?>
    				<td><?=$dadosmun['estuf']?></td>
    			<?} ?>
    		</tr>
    		<?if($_SESSION['projovemcampo']['muncod']){?>
	    		<tr>
	    			<td class="SubTituloDireita" width="30%">Município:</td>
	    			<td><?=$dadosmun['municipio']?></td>
	    		</tr>
    		<?}?>
    		<tr>
    			<td class="SubTituloDireita">Código INEP</td>
				<td><? echo campo_texto('entcodent', 'S', 'S', 'Código INEP', 11, 10, "##########", "", '', '', 0, 'id="entcodent"', '', $entcodent, 'buscarEscolaPorINEP(this.value,\'\');' ); ?>
				<input type="button" name="buscar" value="Buscar" <?=$disable?>
						onclick="var today = new Date();
						displayMessage('projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&requisicao=buscarEscolas&nocache='+today);">
				</td>
    		</tr>
    		<?if($_REQUEST['turid']){?>
				<tr>
	    			<td class="SubTituloDireita">Turma</td>
	    			<td><?=$dadosturma['turdescricao'] ?></td>
	    		</tr>
    		<?} ?>
    		<tr>
    			<td class="SubTituloDireita">Quantidade de alunos</td>
    			<td><? echo campo_texto('turqtdalunosprevistos', 'S', $bloq, 'Quantidade de alunos', 8, 7, "##########", "", '', '', 0, 'id="turqtdalunosprevistos"', '', $turqtdalunosprevistos, '' ); ?></td>
			</tr>
    		<tr>
    			<td class="SubTituloDireita">Nome da escola</td>
    			<td id="td_entnome"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Tipo de orgão</td>
    			<td id="td_tpcdesc"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Localização</td>
    			<td id="td_tpldesc"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Endereço</td>
    			<td id="td_endlog"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Número</td>
    			<td id="td_endnum"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Complemento</td>
    			<td id="td_endcom"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Bairro</td>
    			<td id="td_endbai"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">CEP</td>
    			<td id="td_endcep"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Município</td>
    			<td id="td_mundescricao"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">UF</td>
    			<td id="td_estuf"></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Telefone</td>
    			<td id="td_enttelefone"></td>
    		</tr>
    		<?if(!in_array(PFL_CONSULTA,$perfis)){?>
	    		<tr>
	    			<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="gravarTurma();" <?=$disable?>></td>
	    		</tr>
    		<?}?>
    	</table>
    	</form>
    	<?
    	monta_titulo('Turmas da Localidade','');
    	if(!in_array(PFL_CONSULTA,$perfis)&&$estadodocumento==ESD_EMELABORACAO){
			$deletar = "<img src=../imagens/excluir.gif title=\"Excluir\" style=cursor:pointer; onclick=excluirTurmaEscola(\'projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma&requisicao=excluirTurma&turid='||t.turid||'\') >";
    	}
		$editar = "<img src=../imagens/alterar.gif title=\"Editar\" style=cursor:pointer; onclick=editarTurma('|| t.turid ||') >";
    	
    		$sql = "
					SELECT
						'<center> $deletar $editar<center>' as acao,
						'<center>'||entcodent||'<center>' as inep,
						'<center>'||mun.estuf||'<center>' as uf,
						'<center>'||mun.mundescricao||'<center>' as municipio,
						'<center>''<a href=\"javascript:detalharEndereco(' || entcodent || ');\">'||entnome||'</a>''<center>' as escola,
						'<center>'||turdescricao||'<center>' as turma,
						'<center>'|| turqtdalunosprevistos||'<center>' as qtdalunos
					FROM
							projovemcampo.turma t
					INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = t.secaid
					INNER JOIN 	entidade.entidade e ON e.entid = t.entid AND t.turstatus = 'A'
					INNER JOIN entidade.endereco ende ON ende.entid = e.entid
					INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
					WHERE
						a.apcid = {$_SESSION['projovemcampo']['apcid']}
    				ORDER BY 
    					turid";
//     		   ver($sql,d);
    		$cabecalho = array("","Cód. INEP","UF","Município","Escola","Turma", "Quantidade de Estudantes");
    		$db->monta_lista_simples($sql, $cabecalho, 100, 5, 'N', '70%', '', '','','','t');
    	
    	?>
    	<script type="text/javascript">
    	messageObj = new DHTML_modalMessage();	// We only create one object of this class
    	messageObj.setShadowOffset(5);	// Large shadow
    	
    	function displayMessage(url) {
    		messageObj.setSource(url);
    		messageObj.setCssClassMessageBox(false);
    		messageObj.setSize(450,350);
    		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
    		messageObj.display();
    	}
    	
    	function closeMessage() {
    		messageObj.close();	
    	}
    	
    	</script>
    	
    	</body>
    	</html>
    	<?registarUltimoAcesso();
