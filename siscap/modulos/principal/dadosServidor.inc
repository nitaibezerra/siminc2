<?php

if($_POST['carregaServidor']){
    header('content-type: text/html; charset=ISO-8859-1');
    carregaServidorAjax( $_POST['alucpf'] );
    exit;
}

if( $_POST['ajaxMAT'] ){
    header('content-type: text/html; charset=ISO-8859-1');
    $sql = "SELECT no_servidor FROM siscap.tb_cadastro_servidor
            WHERE nu_matricula_siape = '".$_POST['ajaxMAT']."' limit 1";

    echo $db->pegaUm( $sql );
    exit();
}

$_SESSION['siscap']['turid'] = !empty( $_SESSION['siscap']['turid'] ) ? $_SESSION['siscap']['turid'] : $_GET['turid'];
$_SESSION['siscap']['curid'] = !empty( $_SESSION['siscap']['curid'] ) ? $_SESSION['siscap']['curid'] : $_GET['curid'];
$_SESSION['siscap']['tipo'] = !empty( $_SESSION['siscap']['tipo'] ) ? $_SESSION['siscap']['tipo'] : $_GET['tipo'];

validaSessao( $_SESSION['siscap']['turid'], 'principal/cronogramaCurso' );
validaSessao( $_SESSION['siscap']['curid'], 'principal/cronogramaCurso' );

require_once APPRAIZ . "includes/classes/entidades.class.inc";
require_once APPRAIZ . "www/includes/webservice/cpf.php";

$turid = $_SESSION['siscap']['turid'];
$curid = $_SESSION['siscap']['curid'];

if( $_POST['requisicao'] == 'salvar' ){
	extract( $_POST );

	$aluteletrabalho = str_replace(array("(", ")", "-", " "), "", $aluteletrabalho);
	$aluteleresidencial = str_replace(array("(", ")", "-", " "), "", $aluteleresidencial);
	$alutelecelular = str_replace(array("(", ")", "-", " "), "", $alutelecelular);
	$aluteletrabalhochefia = str_replace(array("(", ")", "-", " "), "", $aluteletrabalhochefia);
	$alumatchefia = str_replace(array(".", "-"), "", $alumatchefia);
	$alucpf = str_replace(array(".", "-"), "", $alucpf);

	$boExiste = $db->pegaUm("SELECT count(alucpf) FROM siscap.aluno WHERE alucpf = '$alucpf'");

	if( $boExiste == 0 ){
		$alucpf = trim($alucpf);
		$alucpf = (!empty($alucpf) ? $alucpf : $usucpf);
		$alumatsiape = trim($alumatsiape);
	  	$aluvinculomec = trim($aluvinculomec);
	  	$aluorgao = trim($aluorgao);
	  	$alulotacao = trim($alulotacao);
	  	$alucargo = trim($alucargo);
	  	$alufuncao = trim($alufuncao);
	  	$alunome = trim($alunome);

		$sql = "INSERT INTO siscap.aluno(alucpf, alunome, aluareaatuacao, aluendtrabalho,
  										aluemail, aluteletrabalho, aluteleresidencial, alutelecelular, alugecoodequip, alugecoodproj,
  										alunivel, alucurso, aluarea, alunomechefia, aluemailchefia, aluteletrabalhochefia, alucargofuncaochefia,
  										aluneceateespecial, aluatendespecial, alumatchefia, alumatsiape, aluvinculomec, aluorgao, alulotacao,
  										alucargo, alufuncao)
				VALUES ('$alucpf', '$alunome', '$aluareaatuacao', '$aluendtrabalho',
  										'$aluemail', '$aluteletrabalho', '$aluteleresidencial', '$alutelecelular', '$alugecoodequip', '$alugecoodproj',
  										'$alunivel', '$alucurso', '$aluarea', '$alunomechefia', '$aluemailchefia', '$aluteletrabalhochefia', '$alucargofuncaochefia',
  										'$aluneceateespecial', '$aluatendespecial', '".trim($alumatchefia)."', '$alumatsiape', '".str_to_upper($aluvinculomec)."',
  										'".str_to_upper($aluorgao)."', '".str_to_upper($alulotacao)."', '".str_to_upper($alucargo)."', '".str_to_upper($alufuncao)."')";
		$db->executar($sql);
	} else {
		$sql = "UPDATE siscap.aluno SET
				  alunome = '$alunome',
				  aluareaatuacao = '$aluareaatuacao',
				  aluendtrabalho = '$aluendtrabalho',
				  aluemail = '$aluemail',
				  aluteletrabalho = '$aluteletrabalho',
				  aluteleresidencial = '$aluteleresidencial',
				  alutelecelular = '$alutelecelular',
				  alugecoodequip = '$alugecoodequip',
				  alugecoodproj = '$alugecoodproj',
				  alunivel = '$alunivel',
				  alucurso = '$alucurso',
				  aluarea = '$aluarea',
				  alunomechefia = '$alunomechefia',
				  aluemailchefia = '$aluemailchefia',
				  aluteletrabalhochefia = '$aluteletrabalhochefia',
				  alucargofuncaochefia = '$alucargofuncaochefia',
				  aluneceateespecial = '$aluneceateespecial',
				  aluatendespecial = '$aluatendespecial',
				  alumatchefia = '$alumatchefia',
				  alumatsiape = '$alumatsiape',
	  			  aluvinculomec = '".str_to_upper($aluvinculomec)."',
	  			  aluorgao = '".str_to_upper($aluorgao)."',
	  			  alulotacao = '".str_to_upper($alulotacao)."',
	  			  alucargo = '".str_to_upper($alucargo)."',
	  	     	  alufuncao = '".str_to_upper($alufuncao)."'
				WHERE alucpf = '$alucpf'";

		$db->executar($sql);
	}

	$sql = "SELECT COUNT(alcid) as total FROM siscap.aluno_curso WHERE alucpf = '$alucpf' and curid = $curid";
	$alcid = $db->pegaUm( $sql );

	if( $alcid == 0 ){
		$sql = "INSERT INTO siscap.aluno_curso(alucpf, curid, alcexpectativas, alcexperiencia, alcconhecimento)
				VALUES ('$alucpf', $curid, '$alcexpectativas', '$alcexperiencia', '$alcconhecimento')";
		$db->executar( $sql );
	}

	$sql = "SELECT COUNT(atuid) as total FROM siscap.aluno_turma WHERE alucpf = '$alucpf' and turid = $turid";
	$registro = $db->pegaUm( $sql );

	if( empty($registro) ){
		$sql = "INSERT INTO siscap.aluno_turma(alucpf, turid)
				VALUES ('$alucpf', $turid) RETURNING atuid";
		$atuid = $db->pegaUm( $sql );
		$_SESSION['siscap']['atuid'] = $atuid;
	}

	if( $_SESSION['siscap']['tipo'] == 'relatorio' ){
		$sql = "UPDATE siscap.aluno_turma SET atusituacao = 'A', atustatus = 'A' WHERE alucpf = '".$alucpf."' and turid = $turid";
		$db->executar($sql);
	}

	if($db->commit()){
		if( $_SESSION['siscap']['tipo'] != 'relatorio' ){
			$db->sucesso( 'principal/fichaInscricao', '&alucpf='.$alucpf );
		} else {
			$db->sucesso( 'relatorio/relatorioTurmas', '&turid='.$turid );
		}
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'siscap.php?modulo=principal/dadosServidor&acao=A';
			 </script>";
		exit();
	}
}

require_once APPRAIZ . "www/includes/webservice/cpf.php";
include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Dados do Servidor', '' );

$sql = "SELECT alu.alunome, alu.alucpf, alu.aluareaatuacao, alu.aluendtrabalho, alu.alumatchefia,
  			alu.aluemail, alu.aluteletrabalho, alu.aluteleresidencial, alu.alutelecelular, alu.alugecoodequip, alu.alugecoodproj,
  			alu.alunivel, alu.alucurso, alu.aluarea, alu.alunomechefia, alu.aluemailchefia, alu.aluteletrabalhochefia, alu.alucargofuncaochefia,
  			alu.aluneceateespecial, alu.aluatendespecial, alu.alumatsiape, alu.aluvinculomec, alu.aluorgao, alu.alulotacao, alu.alucargo, alu.alufuncao,
            ac.alcconhecimento, ac.alcexperiencia, ac.alcexpectativas, usu.usudatanascimento,
            case when usu.ususexo = 'M' then 'Masculino' else 'Feminino' end ususexo, usu.usucpf,
            cs.nu_matricula_siape, cs.co_uorg_lotacao_servidor, cs.no_servidor, cs.co_funcao, cs.co_nivel_funcao, cs.ds_funcao,
            cs.ds_situacao_servidor, cs.co_orgao_lotacao_servidor, cs.sg_unidade_organizacional, cs.ds_orgao, cs.ds_cargo_emprego
		FROM siscap.aluno alu
			inner join seguranca.usuario usu on usu.usucpf = alu.alucpf
			left join (SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor,
						  co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor,
						  co_orgao_lotacao_servidor, sg_unidade_organizacional,
						  ds_orgao, ds_cargo_emprego, nu_cpf
						FROM siscap.tb_cadastro_servidor) cs on cs.nu_cpf = usu.usucpf
			left join siscap.aluno_curso ac on ac.alucpf = alu.alucpf and ac.curid = $curid
		WHERE alu.alucpf = '".$_SESSION['usucpf']."'";

if( $_SESSION['siscap']['tipo'] != 'relatorio' ) $arDados = $db->pegaLinha( $sql );

$arDados = $arDados ? $arDados : array();
extract( $arDados );

if(!isset($_GET['tipo']) && !count($arDados)){

    $cpf = $_SESSION['usucpf'];
    echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script>
          <script>

            jQuery.noConflict();

            jQuery(document).ready(function(){

                var cpf_aluno = '{$cpf}';
                var comp  = new dCPF();
                comp.buscarDados( cpf_aluno );

                if (comp.dados.no_pessoa_rf != ''){
                    jQuery('#alucpf').val( cpf_aluno );
                    jQuery('#nome').html( comp.dados.no_pessoa_rf );
                    jQuery('#alunome').val( comp.dados.no_pessoa_rf );
                    var dataNasc = comp.dados.dt_nascimento_rf.substr(6,2) +'/'+ comp.dados.dt_nascimento_rf.substr(4,2) +'/'+ comp.dados.dt_nascimento_rf.substr(0,4);

                    jQuery('#datanasc').html( dataNasc );
                    if( comp.dados.sg_sexo_rf == 'M' ) jQuery('#sexo').html('Masculino');
                    if( comp.dados.sg_sexo_rf == 'F' ) jQuery('#sexo').html('Feminino');

                    carregaDadosSiape(cpf_aluno);
                }
            });
         </script>";
}

$aluemailconf 			= $aluemail;
$aluemailchefiaconf 	= $aluemailchefia;

$aluteletrabalho 		= trim($aluteletrabalho);
$aluteleresidencial 	= trim($aluteleresidencial);
$alutelecelular 		= trim($alutelecelular);
$aluteletrabalhochefia 	= trim($aluteletrabalhochefia);
$aluemailchefia 		= trim($aluemailchefia);
$aluemail		 		= trim($aluemail);
$alumatchefia	 		= trim($alumatchefia);

$aluteletrabalho 		= (!empty( $aluteletrabalho ) ? "(" . substr( $aluteletrabalho, 0, 0) . substr( $aluteletrabalho, 0, 2).") " . substr( $aluteletrabalho, 2, 4) . "-" . substr( $aluteletrabalho, 6) : '');
$aluteleresidencial 	= (!empty($aluteleresidencial) ? "(" . substr( $aluteleresidencial, 0, 0) . substr( $aluteleresidencial, 0, 2).") " . substr( $aluteleresidencial, 2, 4) . "-" . substr( $aluteleresidencial, 6) : '');
$alutelecelular 		= (!empty($alutelecelular) ? "(" . substr( $alutelecelular, 0, 0) . substr( $alutelecelular, 0, 2).") " . substr( $alutelecelular, 2, 4) . "-" . substr( $alutelecelular, 6) : '');
$aluteletrabalhochefia 	= (!empty($aluteletrabalhochefia) ? "(" . substr( $aluteletrabalhochefia, 0, 0) . substr( $aluteletrabalhochefia, 0, 2).") " . substr( $aluteletrabalhochefia, 2, 4) . "-" . substr( $aluteletrabalhochefia, 6) : '');

$aluemailchefia 	= ( !empty($aluemailchefia) ? $aluemailchefia : '' );
$aluemailchefiaconf = ( !empty($aluemailchefiaconf) ? $aluemailchefiaconf : '' );
$aluemail 			= ( !empty($aluemail) ? $aluemail : '' );
$aluemailconf 		= ( !empty($aluemailconf) ? $aluemailconf : '' );
$usucpf = formatar_cpf($usucpf);

$boUnidade = 'N';
$boLocacao = 'N';
$boCargo = 'N';

if( empty($nu_matricula_siape) ) $nu_matricula_siape = campo_texto('alumatsiape', 'N', 'S', 'Matr&iacute;cula SIAPE', 7, 15, '[#]', '', '', '', 0, 'id=alumatsiape', '', $alumatsiape );
if( empty($ds_situacao_servidor) ) $ds_situacao_servidor = campo_texto('aluvinculomec', 'N', 'S', 'V&iacute;nculo com o MEC', 30, 20, '', '', '', '', 0, 'id=aluvinculomec', '', $aluvinculomec );
if( empty($ds_orgao) ){
	$ds_orgao = campo_texto('aluorgao', 'S', 'S', 'Orgão', 60, 40, '', '', '', '', 0, 'id=aluorgao', '', $aluorgao );
	$boUnidade = 'S';
}
if( empty($sg_unidade_organizacional) ){
	$sg_unidade_organizacional = campo_texto('alulotacao', 'S', 'S', 'Lota&ccedil;&atilde;o', 60, 40, '', '', '', '', 0, 'id=alulotacao', '', $alulotacao );
	$boLocacao = 'S';
}
if( empty($ds_cargo_emprego) ) {
	$ds_cargo_emprego = campo_texto('alucargo', 'S', 'S', 'Cargo', 60, 40, '', '', '', '', 0, 'id=alucargo', '', $alucargo );
	$boCargo = 'S';
}
if( (empty($ds_funcao) && empty($co_nivel_funcao)) ){
	$ds_funcao = campo_texto('alufuncao', 'N', 'S', 'Fun&ccedil;&atilde;o', 60, 40, '', '', '', '', 0, 'id=alufuncao', '', $alufuncao );
} else {
	$ds_funcao = $ds_funcao . ' - ' . $co_nivel_funcao;
}

if( empty($aluatendespecial) ){
	$display = "style=\"display: none\";";
} else {
	$display = "style=\"display: ''\";";
}

$habilita = 'S';
if( $_SESSION['siscap']['tipo'] != 'relatorio' ) $habilita = 'N';
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="usucpf" id="usucpf" value="<?=$usucpf; ?>">
	<input type="hidden" name="alunome" id="alunome" value="<?=$alunome; ?>">
	<input type="hidden" name="tiporequisicao" id="tiporequisicao" value="<?=$_SESSION['siscap']['tipo']; ?>">

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<th colspan="2">Dados do Servidor</th>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">CPF:</td>
			<td colspan="6" width="35%"><?
				echo campo_texto('alucpf', 'S', $habilita, 'CPF', 20, 14, '###.###.###-##', '', '', '', 0, "id='alucpf' class='normal classcpf'", '', $usucpf, '' );
			?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome Completo:</td>
			<td><div id="nome"><?=$alunome; ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Data de Nascimento:</td>
			<td><div id="datanasc"><?=formata_data($usudatanascimento); ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Sexo:</td>
			<td><div id="sexo"><?=$ususexo; ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Matrícula SIAPE:</td>
			<td><div id="matricula"><?=(empty($nu_matricula_siape) ? trim($alumatsiape) : trim($nu_matricula_siape)); ?></div></td>
		</tr>
		<tr id="vinculotr" style="display: ''">
			<td class="subtitulodireita">Vínculo com o MEC:</td>
			<td><div id="vinculo"><?=(empty($ds_situacao_servidor) ? $aluvinculomec : $ds_situacao_servidor); ?></div></td>
		</tr>
		<tr id="orgaotr" style="display: ''">
			<td class="subtitulodireita">Orgão:</td>
			<td><div id="orgao"><?=(empty($ds_orgao) ? $aluorgao : $ds_orgao); ?>
				<input type="hidden" name="bolocacao" id="bolocacao" value="<?=$boLocacao; ?>"></div></td>
		</tr>
		<tr id="unidadetr" style="display: ''">
			<td class="subtitulodireita">Lotação:</td>
			<td><div id="unidade"><?=(empty($sg_unidade_organizacional) ? $alulotacao : $sg_unidade_organizacional); ?>
				<input type="hidden" name="bounidade" id="bounidade" value="<?=$boUnidade;?>"></div></td>
		</tr>
		<tr id="cargotr" style="display: ''">
			<td class="subtitulodireita">Cargo:</td>
			<td><div id="cargo"><?=(empty($ds_cargo_emprego) ? $alucargo : $ds_cargo_emprego);?>
				<input type="hidden" name="bocargo" id="bocargo" value="<?=$boCargo; ?>"></div></td>
		</tr>
		<tr id="funcaotr" style="display: ''">
			<td class="subtitulodireita">Função:</td>
			<td><div id="funcao"><?=(empty($ds_funcao) ? $alufuncao : $ds_funcao . ' - ' . $co_nivel_funcao);?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área de Atuação:</td>
			<td>
				<?
				echo campo_texto('aluareaatuacao', 'S', 'S', 'Área de Atuação', 80, 100, '', '', '', '', 0, 'id=aluareaatuacao' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Endereço do Trabalho:</td>
			<td>
				<?
				echo campo_texto('aluendtrabalho', 'S', 'S', 'Endereço do Trabalho', 80, 100, '', '', '', '', 0, 'id=aluendtrabalho' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">E-mail Instituicional:</td>
			<td>
				<?
				echo campo_texto('aluemail', 'S', 'S', 'E-mail Instituicional', 80, 100, '', '', '', '', 0, 'id=aluemail' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Confirmar E-mail:</td>
			<td>
				<?
				echo campo_texto('aluemailconf', 'S', 'S', 'Confirmar E-mail', 80, 100, '', '', '', '', 0, 'id=aluemailconf' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Telefone do Trabalho:</td>
			<td>
				<?
				echo campo_texto('aluteletrabalho', 'S', 'S', 'Telefone do Trabalho', 16, 14, '[#]', '', '', '', 0, 'id=aluteletrabalho', "formataTelefone('formulario', 'aluteletrabalho', event);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Residencial:</td>
			<td>
				<?
				echo campo_texto('aluteleresidencial', 'S', 'S', 'Residêncial', 16, 14, '[#]', '', '', '', 0, 'id=aluteleresidencial', "formataTelefone('formulario', 'aluteleresidencial', event);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Celular:</td>
			<td>
				<?
				echo campo_texto('alutelecelular', 'S', 'S', 'Celular', 16, 14, '[#]', '', '', '', 0, 'id=alutelecelular', "formataTelefone('formulario', 'alutelecelular', event);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Gerencia ou coordena equipes?:</td>
			<td>
				<input name="alugecoodequip" value="S" id="alugecoodequipS" <?=($alugecoodequip == 'S' ? 'checked="checked"' : ''); ?> type="radio" title="Gerencia ou coordena equipes"><label for="alugecoodequipS">Sim</label>
				<input name="alugecoodequip" value="N" id="alugecoodequipN" <?=($alugecoodequip == 'N' ? 'checked="checked"' : ''); ?> type="radio" title="Não"><label for="alugecoodequipN">Não</label>
				<img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Gerencia ou coordena projetos?:</td>
			<td>
				<input name="alugecoodproj" value="S" id="alugecoodprojS" <?=($alugecoodproj == 'S' ? 'checked="checked"' : ''); ?> type="radio" title="Gerencia ou coordena projetos"><label for="alugecoodprojS">Sim</label>
				<input name="alugecoodproj" value="N" id="alugecoodprojN" <?=($alugecoodproj == 'N' ? 'checked="checked"' : ''); ?> type="radio" title="Não"><label for="alugecoodprojN">Não</label>
				<img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
			</td>
		</tr>
		<tr>
			<th colspan="2">Escolaridade</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Nível:</td>
			<td>
				<?
				$arNivel = array(
								array("codigo" => 'ME', "descricao" => 'Médio' ),
								array("codigo" => 'SU', "descricao" => 'Superior' ),
								array("codigo" => 'PG', "descricao" => 'Pós Graduação' ),
								array("codigo" => 'MD', "descricao" => 'Mestrado' ),
								array("codigo" => 'DO', "descricao" => 'Doutorado' )
							);

				$db->monta_combo("alunivel", $arNivel, 'S', 'Selecione...', '', '', '', '150', 'S', 'alunivel', '', '', 'Nível' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Curso:</td>
			<td>
				<?
				echo campo_texto('alucurso', 'S', 'S', 'Curso', 80, 100, '', '', '', '', 0, 'id=alucurso' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área:</td>
			<td>
				<?
				echo campo_texto('aluarea', 'S', 'S', 'Área', 80, 100, '', '', '', '', 0, 'id=aluarea' );
				?>
			</td>
		</tr>
		<?if( $_SESSION['siscap']['tipo'] != 'relatorio' ){ ?>
		<tr>
			<th colspan="2">Informações sobre a Chefia Imediata</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Matrícula:</td>
			<td>
				<?
				echo campo_texto('alumatchefia', 'S', 'S', 'Matrícula da Chefia Imediata', 30, 11, '', '', '', '', 0, "id='alumatchefia'", '', '', 'buscaNome(this.value);' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome Completo:</td>
			<td>
				<?
				echo campo_texto('alunomechefia', 'S', 'N', 'Nome Completo', 80, 100, '', '', '', '', 0, 'id=alunomechefia' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">E-mail:</td>
			<td>
				<?
				echo campo_texto('aluemailchefia', 'S', 'S', 'E-mail', 50, 40, '', '', '', '', 0, 'id=aluemailchefia' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Confirmar E-mail:</td>
			<td>
				<?
				echo campo_texto('aluemailchefiaconf', 'S', 'S', 'Confirmar E-mail', 80, 40, '', '', '', '', 0, 'id=aluemailchefiaconf' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Telefone de Trabalho:</td>
			<td>
				<?
				echo campo_texto('aluteletrabalhochefia', 'S', 'S', 'Telefone de Trabalho', 16, 14, '[#]', '', '', '', 0, 'id=aluteletrabalhochefia', "formataTelefone('formulario', 'aluteletrabalhochefia', event);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cargo / Função:</td>
			<td>
				<?
				echo campo_texto('alucargofuncaochefia', 'S', 'S', 'Cargo / Função', 80, 100, '', '', '', '', 0, 'id=alucargofuncaochefia' );
				?>
			</td>
		</tr>
		<tr>
			<th colspan="2">Atendimento Especial</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Necessita de atendimento especial durante o período de realização do curso?</td>
			<td>
				<input name="aluneceateespecial" value="S" id="aluneceateespecialS" onclick="verificaTipoAtendimento();" <?=($aluneceateespecial == 'S' ? 'checked="checked"' : ''); ?> type="radio" title="Necessita de atendimento especial durante o período de realização do curso?"><label for="aluneceateespecialS">Necessito de atendimento especial</label>
				<input name="aluneceateespecial" value="N" id="aluneceateespecialN" onclick="verificaTipoAtendimento();" <?=($aluneceateespecial == 'N' ? 'checked="checked"' : ''); ?> type="radio" title="Não"><label for="aluneceateespecialN">Não necessito de atendimento especial</label>
				<img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
			</td>
		</tr>
		<tr id="especial" <?=$display; ?>>
			<td class="subtitulodireita">Este espaço é destinado a descrição das necessidades especiais durante o período de realização do curso:</td>
			<td>
				<?
				echo campo_textarea('aluatendespecial', 'N', 'S', '', 80, 5, 800, '', '', '', '', 'Publicações');
				?>
			</td>
		</tr>
		<tr>
			<th colspan="2">Conhecimento, experiência e expectativas em relação ao curso/evento</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Qual é o seu conhecimento sobre o tema da capacitação?:</td>
			<td>
				<?
				$arConhec = array(
								array("codigo" => 'NE', "descricao" => 'Nenhum' ),
								array("codigo" => 'BA', "descricao" => 'Básico' ),
								array("codigo" => 'ID', "descricao" => 'Intermediário' ),
								array("codigo" => 'AV', "descricao" => 'Avançado' )
							);
				$db->monta_combo("alcconhecimento", $arConhec, 'S', 'Selecione...', '', '', '', '265', 'S', 'alcconhecimento', '', '', 'Qual é o seu conhecimento sobre o tema da capacitação?' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Qual é o sua experiência relacionada ao tema?:</td>
			<td>
				<?
				$arExp = array(
								array("codigo" => 'MU', "descricao" => 'Muita' ),
								array("codigo" => 'PO', "descricao" => 'Pouca' ),
								array("codigo" => 'NE', "descricao" => 'Nenhuma' )
							);
				$db->monta_combo("alcexperiencia", $arExp, 'S', 'Selecione...', '', '', '', '265', 'S', 'alcexperiencia', '', '', 'Qual é o sua experiência relacionada ao tema?' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Este espaço é destinado a descrição de sua expectativa em relação ao curso que você pretende participar:</td>
			<td>
				<?
				echo campo_textarea('alcexpectativas', 'S', 'S', '', 80, 5, 800, '', '', '', '', 'Descrição de sua experiência em relação ao curso');
				?>
			</td>
		</tr>
		<?} ?>
		<tr bgcolor="#D0D0D0">
            <td></td>
            <td style="text-align: left">
				<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.history.back(-1);" />
				<input type="button" id="btAvancar" value="Avançar" onclick="salvarDadosServidor();" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript">
jQuery.noConflict();

jQuery(document).ready(function(){

	jQuery('input.classcpf').live('change',function(){
		if( !validar_cpf( jQuery(this).val()  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			jQuery(this).val('');
			return false;
		}
		var comp  = new dCPF();
		limpaCampos();
		comp.buscarDados( jQuery(this).val() );

		if (comp.dados.no_pessoa_rf != ''){
			jQuery('#nome').html( comp.dados.no_pessoa_rf );
			jQuery('#alunome').val( comp.dados.no_pessoa_rf );
			var dataNasc = comp.dados.dt_nascimento_rf.substr(6,2) +'/'+ comp.dados.dt_nascimento_rf.substr(4,2) +'/'+ comp.dados.dt_nascimento_rf.substr(0,4);

			jQuery('#datanasc').html( dataNasc );
			if( comp.dados.sg_sexo_rf == 'M' ) jQuery('#sexo').html('Masculino');
			if( comp.dados.sg_sexo_rf == 'F' ) jQuery('#sexo').html('Feminino');

			carregaDadosSiape( jQuery(this).val() );
		}
	});
	var comp  = new dCPF();
	comp.buscarDados( $('usucpf').getValue() );
	if (comp.dados.no_pessoa_rf != ''){
		jQuery('#nome').html( comp.dados.no_pessoa_rf );
		jQuery('#alunome').val( comp.dados.no_pessoa_rf );
		var dataNasc = comp.dados.dt_nascimento_rf.substr(6,2) +'/'+ comp.dados.dt_nascimento_rf.substr(4,2) +'/'+ comp.dados.dt_nascimento_rf.substr(0,4);

		jQuery('#datanasc').html( dataNasc );
		if( comp.dados.sg_sexo_rf == 'M' ) jQuery('#sexo').html('Masculino');
		if( comp.dados.sg_sexo_rf == 'F' ) jQuery('#sexo').html('Feminino');
	}
});

function buscaNome(matricula){
	if( Trim(matricula) ){
		var req = new Ajax.Request('siscap.php?modulo=principal/dadosServidor&acao=A', {
								method:      'post',
								parameters:  '&ajaxMAT=' + matricula,
								asynchronous: false,
								onComplete:   function (res){
	        							$('alunomechefia').setValue(res.responseText);
								  }
							});
	}
}

function carregaDadosSiape(alucpf){

    var html = '';
	var myajax = new Ajax.Request('siscap.php?modulo=principal/dadosServidor&acao=A', {
			        method:     'post',
			        parameters: '&carregaServidor=true&alucpf='+alucpf,
			        asynchronous: false,
			        onComplete: function (res){

		        		var json = res.responseText.evalJSON();

		        		if( json.inscrito == 'N' ){
		        			$('btAvancar').disabled = false;
				        	$('vinculo').update(json.ds_situacao_servidor);
			        	    $('unidade').update(json.sg_unidade_organizacional);
			        	    $('orgao').update(json.ds_orgao);
			        	    $('cargo').update(json.ds_cargo_emprego);
				        	$('matricula').update(json.nu_matricula_siape);
			        	    $('funcao').update(json.ds_funcao);
				        	$('usucpf').setValue(json.usucpf);
				        	$('aluareaatuacao').setValue(json.aluareaatuacao);
				        	$('aluendtrabalho').setValue(json.aluendtrabalho);
				  			$('aluemail').setValue(json.aluemail);
				  			$('aluemailconf').setValue(json.aluemailconf);
				  			$('aluteletrabalho').setValue( formataTelefoneAjax(json.aluteletrabalho) );
				  			$('aluteleresidencial').setValue( formataTelefoneAjax(json.aluteleresidencial) );
				  			$('alutelecelular').setValue( formataTelefoneAjax(json.alutelecelular) );
				  			if( json.alugecoodequip == 'N' ) $('alugecoodequipN').checked = true;
				  			if( json.alugecoodequip == 'S' ) $('alugecoodequipS').checked = true;
				  			if( json.alugecoodproj == 'N' ) $('alugecoodprojN').checked = true;
				  			if( json.alugecoodproj == 'S' ) $('alugecoodprojS').checked = true;

				  			$('alunivel').setValue(json.alunivel);
				  			$('alucurso').setValue(json.alucurso);
				  			$('aluarea').setValue(json.aluarea);
				  			if( $('tiporequisicao').getValue() != 'relatorio' ){
				  				if( json.aluneceateespecial == 'N' ) $('aluneceateespecialN').checked = true;
				  				if( json.aluneceateespecial == 'S' ) $('aluneceateespecialS').checked = true;
				  				$('alumatchefia').setValue(json.alumatchefia);
					  			$('alunomechefia').setValue(json.alunomechefia);
					  			$('aluemailchefia').setValue(json.aluemailchefia);
					  			$('aluemailchefiaconf').setValue(json.aluemailchefiaconf);
					  			$('aluteletrabalhochefia').setValue( formataTelefoneAjax(json.aluteletrabalhochefia) );
					  			$('alucargofuncaochefia').setValue(json.alucargofuncaochefia);
					  			$('aluatendespecial').setValue(json.aluatendespecial);
					  			$('alcconhecimento').setValue(json.alcconhecimento);
					  			$('alcexperiencia').setValue(json.alcexperiencia);
					  			$('alcexpectativas').setValue(json.alcexpectativas);
				  			}

			  			} else {

			  			    $('unidade').update(json.sg_unidade_organizacional);
                            $('vinculo').update(json.ds_situacao_servidor);
                            $('unidade').update(json.sg_unidade_organizacional);
                            $('orgao').update(json.ds_orgao);
                            $('cargo').update(json.ds_cargo_emprego);
                            $('matricula').update(json.nu_matricula_siape);

                            alert('Este aluno já está inscrito para este curso.');
                            $('btAvancar').disabled = true;
                            $('alucpf').focus();
			  			}
			        }
			  });
}

function limpaCampos(){
	$('unidade').update('');
//	$('orgao').update('');
//    $('cargo').update('');
    $('matricula').update('');
//    $('funcao').update('');

    $('usucpf').setValue('');
    $('aluareaatuacao').setValue('');
    $('aluendtrabalho').setValue('');
  	$('aluemail').setValue('');
  	$('aluemailconf').setValue('');
  	$('aluteletrabalho').setValue( '' );
  	$('aluteleresidencial').setValue( '' );
  	$('alutelecelular').setValue( '' );
  	$('alugecoodequipN').checked = false;
  	$('alugecoodequipS').checked = false;
  	$('alugecoodprojN').checked = false;
  	$('alugecoodprojS').checked = false;
  	$('alunivel').setValue('');
  	$('alucurso').setValue('');
  	$('aluarea').setValue('');
  	if( $('tiporequisicao').getValue() != 'relatorio' ){
  		$('aluneceateespecialN').checked = false;
  		$('aluneceateespecialS').checked = false;
  		$('alumatchefia').setValue('');
	  	$('alunomechefia').setValue('');
	  	$('aluemailchefia').setValue('');
	  	$('aluemailchefiaconf').setValue('');
	  	$('aluteletrabalhochefia').setValue( '' );
	  	$('alucargofuncaochefia').setValue('');
	  	$('aluatendespecial').setValue('');
	  	$('alcconhecimento').setValue('');
	  	$('alcexperiencia').setValue('');
	  	$('alcexpectativas').setValue('');
  	}
}
function formataTelefoneAjax(vr){
	var tele = '(' + vr.substr(0, 1) + vr.substr(1, 1) + ') ' + vr.substr(2, 4) + '-' + vr.substr(6, 4);
	return tele;
}
function salvarDadosServidor(){

	var nomeform 		= 'formulario';
	var submeterForm 	= false;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();

	//campos = "#aluareaatuacao#aluendtrabalho#aluemail#aluteletrabalho#aluteleresidencial#alutelecelular#alugecoodequip#alugecoodproj#alunivel#alucurso#aluarea#alumatchefia#alunomechefia#aluemailchefia#aluteletrabalhochefia#alucargofuncaochefia#aluneceateespecial#alcconhecimento#alcexperiencia#alcexpectativas";
	//tiposDeCampos 	= "#texto#texto#texto#texto#texto#texto#radio#radio#select#texto#texto#texto#texto#texto#texto#texto#radio#select#select#textarea";

//	if($('bounidade').value == 'S') {
//		campos[0] = "aluorgao";
//		tiposDeCampos[0] = "texto";
//	}
//	if($('bolocacao').value == 'S'){
//		campos[1] = "alulotacao";
//		tiposDeCampos[1] = "texto";
//	}
//	if($('bocargo').value == 'S'){
//		campos[2] = "alucargo";
//		tiposDeCampos[2] = "texto";
//	}


	campos[3] = "aluareaatuacao";
	campos[4] = "aluendtrabalho";
	campos[5] = "aluemail";
	campos[6] = "aluteletrabalho";
	campos[7] = "aluteleresidencial";
	campos[8] = "alutelecelular";
	campos[9] = "alugecoodequip";
	campos[10] = "alugecoodproj";
	campos[11] = "alunivel";
	campos[12] = "alucurso";
	campos[13] = "aluarea";
	if( $('tiporequisicao').getValue() != 'relatorio' ){
		campos[14] = "alumatchefia";
		campos[15] = "alunomechefia";
		campos[16] = "aluemailchefia";
		campos[17] = "aluteletrabalhochefia";
		campos[18] = "alucargofuncaochefia";
		campos[19] = "aluneceateespecial";
		campos[20] = "alcconhecimento";
		campos[21] = "alcexperiencia";
		campos[22] = "alcexpectativas";
	}
	campos[23] = "aluorgao";


	tiposDeCampos[3] = "texto";
	tiposDeCampos[4] = "texto";
	tiposDeCampos[5] = "texto";
	tiposDeCampos[6] = "texto";
	tiposDeCampos[7] = "texto";
	tiposDeCampos[8] = "texto";
	tiposDeCampos[9] = "radio";
	tiposDeCampos[10] = "radio";
	tiposDeCampos[11] = "select";
	tiposDeCampos[12] = "texto";
	tiposDeCampos[13] = "texto";
	if($('tiporequisicao').getValue() != 'relatorio'){
		tiposDeCampos[14] = "texto";
		tiposDeCampos[15] = "texto";
		tiposDeCampos[16] = "texto";
		tiposDeCampos[17] = "texto";
		tiposDeCampos[18] = "texto";
		tiposDeCampos[19] = "radio";
		tiposDeCampos[20] = "select";
		tiposDeCampos[21] = "select";
		tiposDeCampos[22] = "textarea";
	}
	tiposDeCampos[23] = "texto";

	if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){

        if($('alunome').value == ''){
            alert('O campo nome não pode estar em branco!');
            return false;
        }

		if( (!validaEmail( $F('aluemail') ) ) || ($('aluemail').value.substr(0, 1) == '@') ){
			alert('O e-mail informado está inválido.');
			$('aluemail').focus();
			return false;
		}
		if( (!validaEmail( $F('aluemailconf') ) ) || ($('aluemailconf').value.substr(0, 1) == '@') ){
			alert('O e-mail informado está inválido.');
			$('aluemailconf').focus();
			return false;
		}
		if( $('tiporequisicao').getValue() != 'relatorio' && ( (!validaEmail( $F('aluemailchefia') ) ) || ($('aluemailchefia').value.substr(0, 1) == '@') ) ){
			alert('O e-mail informado está inválido.');
			$('aluemailchefia').focus();
			return false;
		}
		if( $('tiporequisicao').getValue() != 'relatorio' && ( (!validaEmail( $F('aluemailchefiaconf') ) ) || ($('aluemailchefiaconf').value.substr(0, 1) == '@') ) ){
			alert('O e-mail informado está inválido.');
			$('aluemailchefiaconf').focus();
			return false;
		}
		if( $('aluemailconf').value != $('aluemail').value ){
			alert( 'O E-mail Institucional do servidor, não confere.' );
			$('aluemailconf').focus();
			return false;
		}
		if( $('tiporequisicao').getValue() != 'relatorio' && ( $('aluemailchefiaconf').value != $('aluemailchefia').value ) ){
			alert( 'O E-mail da Chefia Imediata, não confere.' );
			$('aluemailchefiaconf').focus();
			return false;
		}

		$('requisicao').value = 'salvar';
		$('formulario').submit();
	}

}
function verificaTipoAtendimento(){
	if( $('aluneceateespecialS').checked == true ){
		$('especial').style.display = '';
	} else {
		$('especial').style.display = 'none';
	}
}

function formataTelefone( formName, fieldName, event ){
	if (event.keyCode)
		var key = event.keyCode;
    else
	    var key = event.which;

	var vr = document.forms[formName].elements[fieldName].value;
	vr = vr.replace('(','');
	vr = vr.replace(')','');
	vr = vr.replace('-','');
	vr = vr.replace(' ','');
	size = vr.length;

	if (key!=9 && key!=8 && key!=46 && (key<37 || key>40)){
		if (size == 1)
			document.forms[formName].elements[fieldName].value = '(' + vr.substr(0, 1);
		else if (size > 1 && size < 3)
			document.forms[formName].elements[fieldName].value = '(' + vr.substr(0, 1) + vr.substr(1, 2) + ') ';
		else if (size > 5 && size < 7)
			document.forms[formName].elements[fieldName].value = '(' + vr.substr(0, 1) + vr.substr(1, 1) + ') ' + vr.substr(2, 4) + '-';
		else if (size > 8)
			document.forms[formName].elements[fieldName].value = '(' + vr.substr(0, 1) + vr.substr(1, 1) + ') ' + vr.substr(2, 4) + '-' + vr.substr(6, 4);
	}
}
</script>