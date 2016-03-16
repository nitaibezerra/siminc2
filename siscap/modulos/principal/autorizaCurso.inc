<?php
if( $_POST['requisicao'] == 'salvar' ){
	/*$sql = "UPDATE siscap.aluno_turma SET atustatus = 'P' WHERE alucpf in (SELECT alucpf FROM siscap.aluno WHERE alumatchefia = '".$_SESSION['usucpf']."' )";
	$db->executar( $sql );*/
	if( is_array($_POST['atuid']) ){
		foreach ($_POST['atuid'] as $key => $v) {
			$arAtuid = explode( '_', $v );
			
			$atuid = $arAtuid[0];
			$atustatus = 'P';
				
			if( $arAtuid[1] == 'SIM' ){
				$atuid = $arAtuid[0];
				$atustatus = 'A';
			} else if( $arAtuid[1] == 'NAO' ){
				$atuid = $arAtuid[0];
				$atustatus = 'N';
			}
			$sql = "UPDATE siscap.aluno_turma SET atustatus = '$atustatus' WHERE atuid = $atuid";
			$db->executar( $sql );
		}
		if($db->commit()){
			$db->sucesso( 'principal/autorizaCurso', '' );
		} else {
			echo "<script>
					alert('Falha na Operação');
					window.location.href = 'siscap.php?modulo=principal/autorizaCurso&acao=A';
				 </script>";
			exit();
		}
	}
}
include APPRAIZ . 'includes/cabecalho.inc';
print "<br>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Autorizar Servidor', '' );

$sql = "SELECT DISTINCT
			alt.atustatus,
			alt.atuid,
			cse.nu_matricula_siape,
			alu.alucpf,
		    alu.alunome,
		    cur.curdsc,
		    to_char(tur.turdtini, 'DD/MM/YYYY')||' a '||to_char(tur.turdtfim, 'DD/MM/YYYY') as data,
		    tur.turhrini,
		    tur.turhrfim,
		    tur.turcarga
		FROM
			siscap.aluno alu
		    inner join siscap.aluno_turma alt on alt.alucpf = alu.alucpf
		    left join (SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor,
		                        co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor, 
		                        co_orgao_lotacao_servidor, sg_unidade_organizacional,
		                        ds_orgao, ds_cargo_emprego, nu_cpf
		                      FROM siscap.tb_cadastro_servidor) cse on cse.nu_cpf = alu.alucpf
		    inner join siscap.turma tur on tur.turid = alt.turid
		    inner join siscap.curso cur on cur.curid = tur.curid
		    inner join siscap.area ar on ar.areid = cur.areid
		WHERE
			alt.atusituacao = 'A'
			and alt.atustatus = 'P'
		    and alu.alumatchefia in (SELECT nu_matricula_siape FROM siscap.tb_cadastro_servidor WHERE nu_cpf = '{$_SESSION['usucpf']}')";

$arDados = $db->carregar( $sql );
$arDados = $arDados ? $arDados : array();

$arRegistro = array();
foreach ($arDados as $key => $v) {
	
	$checkA = '';
	$checkN = '';
	if( $v['atustatus'] == 'A' ) $checkA = 'checked';
	if( $v['atustatus'] == 'N' ) $checkN = 'checked';
	
	$sim = '<center><input type="checkbox" '.$checkA.' id="atuid['.$v['atuid'].'_SIM'.']" name="atuid[]" onclick="validaSelecao(this);" value="'.$v['atuid'].'_SIM'.'" /><input type="hidden" id="requisicao" name="requisicao" value="salvar"></center>';
	$nao = '<center><input type="checkbox" '.$checkN.' id="atuid['.$v['atuid'].'_NAO'.']" name="atuid[]" onclick="validaSelecao(this);" value="'.$v['atuid'].'_NAO'.'" /><input type="hidden" id="requisicao" name="requisicao" value="salvar"></center>';
	
	$arHoraIni = explode( ':', $v['turhrini'] );
	$arHoraFim = explode( ':', $v['turhrfim'] );

	$horaini = $arHoraIni[0].'h'.$arHoraIni[1];
	$horafim = $arHoraFim[0].'h'.$arHoraFim[1];
	
	$arRegistro[$key] = array(
							"sim" => $sim,
							"nao" => $nao,
							"matricula" => $v['nu_matricula_siape'],
							"cpf" => formatar_cpf($v['alucpf']),
							"nome" => $v['alunome'],
							"curso" => $v['curdsc'],
							"data" => $v['data'],
							"periodo" => $horaini.' a '.$horafim,
							"carga" => $v['turcarga'].'h/a',
						);
}


$cabecalho = array( "Sim", "Não", "Matricula SIAPE", "CPF", "Nome", "Curso", "Período", "Horário", "Carga");
//$db->monta_lista_array($arRegistro, $cabecalho, 25, 10, '', 'center', '');	
$db->monta_lista($arRegistro, $cabecalho, 5000000, 10, 'N', 'center', '', 'formlibera', '','');
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form method="post" id="formulario" name="formulario">	
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr bgcolor="#D0D0D0">
			<td colspan="6" style="text-align: center">
				<input type="button" value="Salvar" id="btnSim" name="btnSim" onclick="autorizarAluno();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
function autorizarAluno(){
	var form = document.getElementById('formlibera');
	var Checkbox = false;
	
	for (i=0;i<form.elements.length;i++){
		if(form.elements[i].type == "checkbox"){
			var checarCheckbox = true;
			if (form.elements[i].checked == true ) { 
				Checkbox = true;
			}
		}
	}
	if( Checkbox == false ){
		alert('Selecione pelo menos uma autorização!');
		return false;
	}
	form.submit();
}
function validaSelecao(id){
	var ar = id.value.split('_');

	if( ar[1] == 'NAO' ){
		if( $( 'atuid['+ar[0]+'_NAO]').checked == false ){
			$( 'atuid['+ar[0]+'_NAO]').checked = false;
			$( 'atuid['+ar[0]+'_SIM]').checked = false;
		} else {
			$( 'atuid['+ar[0]+'_NAO]').checked = true;
			$( 'atuid['+ar[0]+'_SIM]').checked = false;
		}
	} 
	if( ar[1] == 'SIM' ){
		if( $( 'atuid['+ar[0]+'_SIM]').checked == false ){
			$( 'atuid['+ar[0]+'_NAO]').checked = false;
			$( 'atuid['+ar[0]+'_SIM]').checked = false;
		} else {
			$( 'atuid['+ar[0]+'_NAO]').checked = false;
			$( 'atuid['+ar[0]+'_SIM]').checked = true;
		}
	}
}
</script>