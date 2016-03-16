<?php

//verifica session
verificaSessao();



//verifica permissão
/*$vPermissao = 'N';
$vpflcod = arrayPerfil();
if( in_array(PERFIL_SUPER_USUARIO, $vpflcod) ){
	$vPermissao = 'S';
}*/
$sql = "select docid from escolasexterior.escolasexterior where eexid = ".$_SESSION['eexid']; // Retorna o docid da escola
$docid = $db->pegaUm($sql);
$esdid = pegaEstadoAtual( $docid ); // Pega o estado atual da escola

//verifica permissão
$vPermissao = 'N';
$vpflcod = arrayPerfil();
if($esdid == WF_ESDID_EM_CADASTRAMENTO && (in_array(PERFIL_SUPER_USUARIO, $vpflcod) || in_array(PERFIL_CADASTRADOR, $vpflcod) || in_array(PERFIL_ADMINISTRADOR, $vpflcod))){
	$vPermissao = 'S';
}


// associa
switch( $_POST['requisicao'] ){
	case 'associaDisciplinaSeriePessoa':
		$dcpid = $_POST['dcpid'];
		$serid = $_POST['serid'];
		$pecid = $_POST['pecid'];

		$sql = "
			INSERT INTO escolasexterior.pessoadisciplina
			(
				pecid,
	        	dcpid
	        ) VALUES (
	        	".$pecid.",
	        	".$dcpid."
	        ) returning apdid
		";
		$apdid = $db->pegaUm($sql);
		$db->commit();

		$sql = "
			INSERT INTO escolasexterior.disciplinaserie
			(
				apdid,
	  			serid
			) VALUES (
				".$apdid.",
				".$serid."
			)
		";
		$db->executar( $sql );
		$db->commit();

		exit('ok');
		break;

	case 'exclui_apdid':
		$sql = "
			DELETE FROM escolasexterior.disciplinaserie
			WHERE apdid = ".$_POST['apdid']."
		";
		$db->executar( $sql );
		$db->commit();
		$sql = "
			DELETE FROM escolasexterior.pessoadisciplina
			WHERE apdid = ".$_POST['apdid']."
		";
		$db->executar( $sql );
		$db->commit();

		exit('ok');
		break;

}


//excluir pessoa
if($_REQUEST['excluir']=='1' && $_REQUEST['peeid']){
	
	//excluir todos os anexos da pessoa
	$sql = "UPDATE escolasexterior.pessoalescolaarquivo SET	pesstatus = 'I' 
			WHERE peeid = {$_REQUEST['peeid']}";
	$db->executar($sql);
	
	$sql = "UPDATE public.arquivo SET arqstatus = 'I' 
			WHERE arqid in (select arqid from escolasexterior.pessoalescolaarquivo where peeid = {$_REQUEST['peeid']} )";
	$db->executar($sql);
	
	
	//exclui a pessoa
	$sql = "UPDATE escolasexterior.pessoalescola SET peestatus = 'I'
			WHERE peeid = ".$_REQUEST['peeid'];
	$db->executar($sql);
	
	$db->commit();	
	
	print '<script>
			alert("Operação realizada com sucesso!");
			location.href="escolasexterior.php?modulo=principal/cadastroPessoal&acao=A";
		   </script>';
	exit();
}


//submit
if($_POST){
	// ver($_POST,d);
	
	//verifica se ja existe a escola
		$andV = $_POST['peeid'] ? ' and peeid not in ('.$_POST['peeid'].') ' : '';
		$sql = "select count(peeid) as total 
				from escolasexterior.pessoalescola 
				where peestatus = 'A' 
				and eexid = ".$_SESSION['eexid']."
				and peenome = '".strtoupper($_POST['peenome'])."'
				$andV";
		$verifica = $db->pegaUm($sql);
		
		if( (int) $verifica > 0 ){
			print "<script>
					alert('Erro: Pessoa já existe!');
					history.back();
			   	  </script>";
			exit();
		}
	//fim verifica
	
	
	$peeid = $_POST['peeid']; 

	// retira referencias decorrentes de haver cargo de professor na lista de cargos
	if( $_POST['professor'] == 'nao' ){
		$_POST['dcpid'] = 'NULL';
        $_POST['serid'] = 'NULL';
	}
		
	if(!$peeid) { //INSERIR
		
		if ($_POST['peedtnasc']){
			$data = "'".formata_data_sql($_POST['peedtnasc'])."'";
		} else {
			$data = "null";
		}
		
		$sql = "INSERT INTO escolasexterior.pessoalescola
			(
				eexid,
            	peenome,
			  	peeemail,
			  	peesexo,
			  	peedtnasc,
			  	peestatus
	        ) VALUES (
	        	".$_SESSION['eexid'].",
	        	'".strtoupper($_POST['peenome'])."',
	        	'".$_POST['peeemail']."',
	        	'".$_POST['peesexo']."',
	        	".$data.",
	        	'A'
	        ) returning peeid";	
		$peeid = $db->pegaUm($sql);
		$db->commit();

		// associa cargos
		foreach ($_POST['cargo'] as $key => $value) {
			$sql = "
				INSERT INTO escolasexterior.pessoalcargo
				(
					peeid,
					cgoid
				) VALUES (
					'{$peeid}',
					'{$value}'
				)
			";
			$db->executar( $sql );
			$db->commit();
		}
	
		unset($_POST);
		
		print '<script>
				alert("Operação realizada com sucesso!");
				location.href="escolasexterior.php?modulo=principal/cadastroPessoal&acao=A";
			   </script>';
		exit();
		
	}else{ //ALTERAR 
		
		$sql = "UPDATE 
					escolasexterior.pessoalescola
				SET 
					peenome = '".strtoupper($_POST['peenome'])."',
					peeemail = '".$_POST['peeemail']."',
					peesexo = '".$_POST['peesexo']."',
					peedtnasc = '".formata_data_sql($_POST['peedtnasc'])."'
				WHERE 
					peeid = ".$peeid;
		$db->executar($sql);
		$db->commit();

		// associa cargos
		foreach ($_POST['cargo'] as $key => $value) {
			$sql1 = "
				SELECT *
				FROM escolasexterior.pessoalcargo
				WHERE peeid = '{$peeid}' AND cgoid = '{$value}'
			";
			// ver($sql1,d);
			$busca = $db->carregar( $sql1 );
			if( !$busca ){
				$sql = "
					INSERT INTO escolasexterior.pessoalcargo
					(
						peeid,
						cgoid
					) VALUES (
						'{$peeid}',
						'{$value}'
					)
				";
			}else{
				$sql = "
					UPDATE escolasexterior.pessoalcargo
					SET pecstatus = 'A'
					WHERE peeid = '{$peeid}' AND cgoid = '{$value}'
				";
			}
			$db->executar( $sql );
			$db->commit();
		}
		// desativa nao presentes
		if( $_POST['cargo'] ){
			$sql = "
				SELECT pecid 
				FROM escolasexterior.pessoalcargo
				WHERE peeid = {$peeid} AND cgoid NOT IN ('".(implode("','",$_POST['cargo']))."') ";
                                //ver($sql,d);
			$listaPecid = $db->carregar( $sql );

			$sql = "
				UPDATE escolasexterior.pessoalcargo
				SET pecstatus = 'I'
				WHERE peeid = '{$peeid}' and cgoid NOT IN ('".(implode("','",$_POST['cargo']))."')
			";
			// ver($sql,d);
			$db->executar( $sql );
			$db->commit();

                        if($listaPecid){
			foreach ($listaPecid as $key => $value) {
				$sql = "
					DELETE FROM escolasexterior.disciplinaserie
					WHERE apdid = ( SELECT apdid FROM escolasexterior.pessoadisciplina WHERE pecid = {$value['pecid']} )
				";
				$db->executar( $sql );
				$db->commit();

				$sql = "
					DELETE FROM escolasexterior.pessoadisciplina
					WHERE pecid = {$value['pecid']}
				";
				$db->executar( $sql );
				$db->commit();
			}
                        }
		}

	
		unset($_POST);
		
		print '<script>
				alert("Operação realizada com sucesso!");
				location.href="escolasexterior.php?modulo=principal/cadastroPessoal&acao=A&peeid='.$peeid.'";
			   </script>';
		exit();
	
	}
	
	
	
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo = "ESCOLAS NO EXTERIOR";

$dsctitulo = 'CADASTRO - DADOS DO PESSOAL<BR><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';


montaAbasEscolasExterior();

monta_titulo( $titulo, $dsctitulo );

cabecalhoEE($_SESSION['peeid']);


//carrega dados para edição
if($_REQUEST['peeid']){
	$sql = "SELECT  
				  	peeid,
				  	peenome,
				  	peeemail,
				  	peesexo,
				  	peedtnasc,
				  	peestatus
		     FROM 
		     		escolasexterior.pessoalescola
		     WHERE 
		     		peeid = {$_REQUEST['peeid']}";
	// ver($sql,d);
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}



?>

<script src="../includes/calendario.js"></script>

<style>
.SerieEDisciplina{
	display:none;
}
</style>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="peeid" value="<?=$peeid?>">
<input type="hidden" name="professor" value="">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Nome da Pessoa:</td>
		<td colspan='2'>
			<?=campo_texto( 'peenome', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
		<td align="right" rowspan="7">
			
				<?php
					if($_SESSION['eexid']){
						$docid = criaDocumento( $_SESSION['eexid'] ); 
						if($docid){
							wf_desenhaBarraNavegacao( $docid , array("eexid" => $_SESSION['eexid']) );
						} 
					}
				?>
			
		</td>
	</tr>
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>E-mail:</td>
		<td colspan='2'>
			<?=campo_texto( 'peeemail', 'S', $vPermissao, '', 50, 100, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Sexo:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								'F' as codigo,
								'Feminino' as descricao
							  union
							  SELECT
								'M' as codigo,
								'Masculino' as descricao";			
				$db->monta_combo("peesexo", $sql_combo, $vPermissao, "Selecione o Sexo", '', '', '', '', 'S', 'peesexo');
			?>
		</td>
	</tr>
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Data de Nascimento:</td>
		<td colspan='2'>
			<?=campo_data('peedtnasc', 'N', $vPermissao, '', 'S' );?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Cargo:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								cgoid as codigo,
								cgodsc as descricao
							  FROM escolasexterior.cargo
							  where cgostatus = 'A'
							  order by 2";
				$cargo = array();
				if( $_GET['peeid'] ){
					$sql_carregados = "
						SELECT
							pc.cgoid as codigo,
							c.cgodsc as descricao
						FROM 
							escolasexterior.pessoalcargo pc
						JOIN escolasexterior.cargo c on c.cgoid = pc.cgoid
						WHERE 
							peeid = {$_GET['peeid']}
							and pecstatus = 'A'
					";
					$cargo = $db->carregar( $sql_carregados );
				}
				combo_popup( 'cargo', $sql_combo, 'Cargo: ','400x400', 0, array(), '', $vPermissao, false,
false, 10, 400, null, null, false, null, null, true, false, 'posComboPopup(this)' );
			?>
		</td>
	</tr>
	<tr class="SerieEDisciplina">
		<td width="32%" class="SubTituloDireita" colspan='2'>Professor em:</td>
		<td colspan="2">
                    <?php
                    if ($_GET['peeid']) {
                        $sql_combo = "SELECT
									pc.pecid as codigo,
									c.cgodsc as descricao,
									c.cgoid as cgoid
								  FROM 
								  	escolasexterior.pessoalcargo pc
								  JOIN escolasexterior.cargo c on c.cgoid = pc.cgoid
								  WHERE pc.pecstatus='A' AND pc.peeid = " . $_GET['peeid'] . " AND c.cgodsc LIKE '%Professor%'
								  ORDER BY 2";
                            $comboPecid = $db->carregar($sql_combo);
                        if ($comboPecid) {
                            echo "<select id='pecid' name='pecid' style='width:auto' class='CampoEstilo obrigatorio' >";
                            echo "<option value=''>Cargo</option>";
                            foreach ($comboPecid as $key => $value) {
                                echo "<option value='" . $value['codigo'] . "' cgoid='" . $value['cgoid'] . "' ";
                                // if( $value['codigo'] == $pecid ) echo " selected ";
                                echo ">" . $value['descricao'] . "</option>";
                            }
                            echo "</select>";
                        }
                        // $db->monta_combo("pecid", $sql_combo, $vPermissao, "Selecione o Cargo", '', '', '', '', 'S', 'pecid');
                    }
                    ?>
			<?
				$sql_combo = "SELECT
								dcpid as codigo,
								dcpdsc as descricao
							  FROM 
							  	escolasexterior.disciplina
							  WHERE dcpstatus='A'
							  ORDER BY 2";			
				$db->monta_combo("dcpid", $sql_combo, $vPermissao, "Selecione a Disciplina", '', '', '', '', 'S', 'dcpid');
			?>
			<?
				$sql_combo = " 
					SELECT
						serid as codigo,
						serdsc as descricao
					FROM 
					 	escolasexterior.serie
					WHERE serstatus='A'
					ORDER BY 2 ";
							
				$db->monta_combo("serid", $sql_combo, $vPermissao, "Selecione a Série", '', '', '', '', 'S', 'serid');
			?>
			<input type="button" name="botao_associacao_disciplina_e_serie" value="Salvar Disciplina e Série" onclick="javascript:associaDisciplinaSeriePessoa();"/>
		</td>
	</tr>
	<tr class="SerieEDisciplina">
		<td width="32%" class="SubTituloDireita" colspan='2'>Lista de Disciplinas:</td>
		<td colspan="2">
			<?
				if($_GET['peeid']){
					$sql = "SELECT
									pd.apdid,
									s.serdsc as serie,
									d.dcpdsc as disciplina,
									c.cgodsc  as cargo
								  FROM 
								  	escolasexterior.pessoadisciplina pd
								  JOIN escolasexterior.disciplinaserie ds on ds.apdid = pd.apdid
								  JOIN escolasexterior.disciplina d on d.dcpid = pd.dcpid
								  JOIN escolasexterior.serie s on s.serid = ds.serid
								  JOIN escolasexterior.pessoalcargo pc on pc.pecid = pd.pecid
								  JOIN escolasexterior.cargo c on c.cgoid = pc.cgoid
								  WHERE dcpstatus='A' and pc.peeid = ".$_GET['peeid']."
								  ORDER BY disciplina";	
					// $db->monta_combo("dcpid", $sql_combo, $vPermissao, "Selecione a Disciplina", '', '', '', '', 'S', 'dcpid');
					// ver($sql);
					$lista = $db->carregar( $sql );
					if( $lista )
					foreach ($lista as $key => $value) {
						echo "
							<img src='../imagens/excluir.gif' style='float:left;cursor:pointer;' apdid='".$value['apdid']."'  class='exclui_apdid' />
							<div style='float:left'>&nbsp;".$value['cargo']." - ".$value['disciplina']." - ".$value['serie']."</div>
							<div style='clear:both;height:4px;'></div>";
					}
				}
			?>
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaFormE();">
						<?}?>
						&nbsp;
						<?php if( !$_GET['peeid'] ){ ?>
							<input type="reset" class="botao" name="btLimpar" id="btLimpar" value="Limpar">
						<?php }else{ ?>
							<input type="button" onclick="window.location.href='/escolasexterior/escolasexterior.php?modulo=principal/cadastroPessoal&acao=A'" class="botao" name="btLimpar" id="btLimpar" value="Limpar">
						<?php } ?>
						&nbsp;
						<?php if( $vPermissao == 'S' ){ ?>
							<input type="button" class="botao" name="btLimpar" id="btNova" value="Cadastrar Nova Pessoa" onclick="location.href='escolasexterior.php?modulo=principal/cadastroPessoal&acao=A';">
						<?php } ?>
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>

</form>


<?php 

	$sql = "
		SELECT
			'<center>' ||
			(CASE WHEN '{$vPermissao}' = 'S' THEN
					 '<a href=\"#\" onclick=\"alterarItem(\'' || p.peeid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirItem(\'' || p.peeid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
				  ELSE
					 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
			END) || '</center>'  as acao,
			peenome as nome,
			peeemail as email,
			case when peesexo = 'F' then
				'Feminino'
				else
				'Masculino'
			end as sexo,
			'<center>'||to_char(peedtnasc::TIMESTAMP,'DD/MM/YYYY')||'</center>' as dtnasc, 
        	array_to_string(array( 
        	 	select c2.cgodsc 
        	 	from escolasexterior.pessoalcargo pc2
        	 	join escolasexterior.cargo c2 on pc2.cgoid = c2.cgoid
        	 	where pc2.peeid = p.peeid and pecstatus = 'A' 
        	),', ') AS cargo,
        	'<center><a href=\"#\" onclick=\"anexarItem(\'' || p.peeid || '\');\" title=\"Anexar Documentos\"><img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" border=\"0\"></a></center>' as docs
		FROM escolasexterior.pessoalescola p
		WHERE p.eexid = {$_SESSION['eexid']}
		and p.peestatus = 'A'
		order by nome ";
                
//                ver($sql,d);

	$cabecalho 	= array( "Ação","Nome","E-mail","Sexo","Data de Nascimento","Cargo","Documentos");
	$db->monta_lista_simples( $sql, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
?>

</body>
</html>

 
<script>

var d = document.formulario;



function validaFormE(){

	if(!validaBranco( d.peenome, 'Nome da Pessoa' ))
	{
		return false;
	}
	if(!validaBranco( d.peeemail, 'E-mail' ))
	{
		return false;
	}
	if(!validaBranco( d.peesexo, 'Sexo' ))
	{
		return false;
	}
	if(!validaBranco( d.peeemail, 'E-mail' ))
	{
		return false;
	}
	/*if(!validaBranco( d.peedtnasc, 'Data de Nascimento' ))
	{
		return false;
	}*/
	if( d.cargo.options.length < 1 ){
		alert('Campo Cargo é obrigatório.');
		return false;
	}else{
		if (d.cargo.options.length == 1 && d.cargo.options[0].value == ''){
			alert('Campo Cargo é obrigatório.');
			return false;
		}

		for( var i = 0; i < document.getElementById('cargo').options.length; i++){
			document.getElementById('cargo').options[i].selected = true;
		}		
	}
		
	d.submit();

}


function alterarItem(id){
	location.href='escolasexterior.php?modulo=principal/cadastroPessoal&acao=A&peeid='+id;
}

function excluirItem(id){
	if(confirm('Deseja realmente excluir esta pessoa?')){
		location.href='escolasexterior.php?modulo=principal/cadastroPessoal&acao=A&excluir=1&peeid='+id;
	}
}

function anexarItem(id){

	w = window.open('?modulo=principal/cadastroPessoalAnexo&acao=A&peeid='+id,'Anexo','scrollbars=yes,resizable=yes,location=no,toolbar=yes,menubar=no,width=780,height=400,left=250,top=125'); 
	w.focus();	
	
}

function posComboPopup( elemento ){
	if( jQuery(elemento).attr('descricao').substring(0,9) == 'Professor' ){
		habilitaSerieEDisciplina();
	}
}

function habilitaSerieEDisciplina(){
	jQuery('.SerieEDisciplina').show();
	jQuery('[name=professor]').val('sim');
}

function desabilitaSerieEDisciplina(){
	jQuery('.SerieEDisciplina').hide();
	jQuery('[name=professor]').val('nao');
}

jQuery(function(){
	// inicializacao
	var arrayValidadorInicializacao = [];
	jQuery.each( jQuery('#cargo').find('option'),function( index, value ){
		arrayValidadorInicializacao.push( jQuery(value).html().substring(0,9) );
	});

	if( in_array('Professor',arrayValidadorInicializacao) ){
		habilitaSerieEDisciplina();
	}else{
		desabilitaSerieEDisciplina();
	}

	// blur
	jQuery('#cargo').blur(function(){
		var arrayValidador = [];
		
		jQuery.each( jQuery(this).find('option'),function( index, value ){
			arrayValidador.push( jQuery(value).html().substring(0,9) );
		});

		// avisa de salvamento de formulario necessário
		jQuery('.SerieEDisciplina').find('td').eq(1).html('<font style="color:red">Salve o formulário para poder associar os cargos às respectivas disciplinas.</font>');

		if( in_array('Professor',arrayValidador) ){
			habilitaSerieEDisciplina();
		}else{
			desabilitaSerieEDisciplina();
		}
	});

	// exclusao de apdid
	jQuery('.exclui_apdid').click(function(){
		if( confirm("Deseja realmente excluir essa Disciplina?") ){
			var apdid = jQuery(this).attr('apdid')
			exclui_apdid( apdid );
		}
	});
});

function in_array(needle, haystack) {
    for(var i in haystack) {
        if(haystack[i] == needle) return true;
    }
    return false;
}

function associaDisciplinaSeriePessoa(){
	if(!validaBranco( d.dcpid, 'Disciplina' ))
	{
		return false;
	}
	if(!validaBranco( d.serid, 'Série' ))
	{
		return false;
	}
	if(!validaBranco( d.pecid, 'Cargo ' ))
	{
		return false;
	}

	var dcpid =  jQuery(d.dcpid).val();
	var serid = jQuery(d.serid).val();
	var pecid = jQuery(d.pecid).val();

	jQuery.ajax({
		url: window.location.href,
		type:"POST",
		data:{requisicao:'associaDisciplinaSeriePessoa',dcpid:dcpid,serid:serid,pecid:pecid},
		success:function( resposta ){
			console.log(resposta);
			if( String(resposta).substring(String(resposta).length-2,String(resposta).length) == 'ok' ){
				alert('Associação feita com sucesso!');
				window.location.reload();

			}else{
				alert('Houve um erro ao salvar, tente novamente mais tarde.');
			}
		}
	});
}

function exclui_apdid( apdid ){
	jQuery.ajax({
		url: window.location.href,
		type: "POST",
		data: {requisicao:'exclui_apdid',apdid:apdid},
		success: function( resposta ){
			if( String(resposta).substring(String(resposta).length-2,String(resposta).length) == 'ok' ){
				alert('Associação desfeita com sucesso!');
				window.location.reload();

			}else{
				alert('Houve um erro ao salvar, tente novamente mais tarde.');
			}
		}
	});
}

</script>