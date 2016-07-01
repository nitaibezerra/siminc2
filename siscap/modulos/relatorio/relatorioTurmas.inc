<?php
set_time_limit(0);
ini_set('memory_limit', '1024M');

unset($_SESSION['siscap']['turid']);
unset($_SESSION['siscap']['curid']);
unset($_SESSION['siscap']['tipo']);

require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatórios de Turmas', '');

if( !empty($_GET['turid']) ){
	$filtro = " and alt.turid = ".$_GET['turid'];
}

$sql = "SELECT t.turturma, c.curdsc, t.turobs, c.curid, t.turstatus FROM siscap.turma t
	inner join siscap.curso c on c.curid = t.curid
WHERE
	t.turid = ".$_GET['turid'];

$arTurma = $db->pegaLinha( $sql );
//$boInscrito = $db->pegaUm( "select count(atuid) from siscap.aluno_turma where turid = ".$_GET['turid']." and alucpf = '{$_SESSION['usucpf']}' and atusituacao = 'A'");

?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td class="subtitulodireita" width="20%">Curso:</td>
		<td><?=$arTurma['curdsc']; ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20%">Turma:</td>
		<td><?=$arTurma['turturma']; ?></td>
	</tr>
	<?php if( $arTurma['turobs'] ){ ?>
	<tr>
		<td class="subtitulodireita" width="20%">Observações:</td>
		<td><?=$arTurma['turobs']; ?></td>
	</tr>
	<?php } ?>
</table>
<?

$sql = "SELECT DISTINCT 
			'<center><img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"visualizarFicha('||".$arTurma['curid']."||',\''||alu.alucpf||'\');\" border=0 alt=\"Ir\" title=\"Visualizar\"></center>' as acao, 
			alu.alunome, substr(alu.alucpf, 0, 4)||'.'||substr(alu.alucpf, 4, 3)||'.'||substr(alu.alucpf, 7, 3)||
		    '-'||substr(alu.alucpf, 10, 2) as cpf, 
		    cse.nu_matricula_siape, cse.co_uorg_lotacao_servidor, cse.no_servidor, cse.co_funcao, cse.co_nivel_funcao, cse.ds_funcao, 
            cse.ds_situacao_servidor, cse.co_orgao_lotacao_servidor, cse.sg_unidade_organizacional, cse.ds_orgao, cse.ds_cargo_emprego,
            alu.alumatsiape, alu.aluvinculomec, alu.aluorgao, alu.alulotacao, alu.alucargo, alu.alufuncao,
		    to_char(alt.atudatainscricao, 'DD/MM/YYYY HH24:MI') as data, 
			case when alt.atustatus = 'P' then 'Pendente' 
				when alt.atustatus = 'A' then 'Autorizado'
				when alt.atustatus = 'N' then 'Não Autorizado' end as autorizacao
		FROM siscap.aluno alu
		    inner join siscap.aluno_turma alt on alt.alucpf = alu.alucpf
		    inner join siscap.turma tur on tur.turid = alt.turid
            left join (SELECT nu_matricula_siape, nu_cpf, co_uorg_lotacao_servidor, no_servidor,
                        co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor, 
                        co_orgao_lotacao_servidor, sg_unidade_organizacional,
                        ds_orgao, ds_cargo_emprego
                      FROM siscap.tb_cadastro_servidor) cse on cse.nu_cpf = alu.alucpf
		WHERE
			alt.atusituacao = 'A'
			$filtro";
			
$arDados = $db->carregar( $sql );
$arDados = ($arDados ? $arDados : array());

$arRegistro = array();
foreach ($arDados as $key => $v) {
	
	$nu_matricula_siape = (empty($v['nu_matricula_siape']) ? $v['alumatsiape'] : $v['nu_matricula_siape']);
	$ds_situacao_servidor = (empty($v['ds_situacao_servidor']) ? $v['aluvinculomec'] : $v['ds_situacao_servidor']);
	$sg_unidade_organizacional = (empty($v['sg_unidade_organizacional']) ? $v['alulotacao'] : $v['sg_unidade_organizacional']);
	//$ds_orgao = (empty($ds_orgao) ? $aluorgao : $ds_orgao);
	$ds_cargo_emprego = (empty($v['ds_cargo_emprego']) ? $v['alucargo'] : $v['ds_cargo_emprego']);
	$ds_funcao = (empty($v['ds_funcao']) ? $v['alufuncao'] : $v['ds_funcao'].$v['co_nivel_funcao']);
	
	$dataBD = explode( ':', $v['data'] );
	$dataFormatada = $dataBD[0].'h'.$dataBD[1];

	$arRegistro[$key] = array(
							"acao" => $v['acao'],
							"nome" => $v['alunome'],
							"cpf" => $v['cpf'],
							"siape" => $nu_matricula_siape.'&nbsp;',
							"lotacao" => $sg_unidade_organizacional,
							"situacao" => $ds_situacao_servidor,
							"cargo" => $ds_cargo_emprego,
							"funcao" => $ds_funcao,
							"incricao" => $dataFormatada,
							"autoriza" => $v['autorizacao'],
						);	
}


$cabecalho = array("Ação", "Nome", "CPF", "SIAPE", "Lotação", "Sit. Funcional", "Cargo", "Função", "Inscrição", "Autorização");
//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
$db->monta_lista_array($arRegistro, $cabecalho, 5000, 20, '', 'center', '');
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center">	
			<?if( $arTurma['turstatus'] == 'IA' ){ ?>				
				<input type="button" id="btInscricao" value="Ficha de Inscrição" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/dadosServidor&acao=A&curid=<?=$arTurma['curid'];?>&turid=<?=$_GET['turid']; ?>&tipo=relatorio'" />
			<?} else {?>
				<input type="button" disabled="disabled" id="btInscricao" value="Ficha de Inscrição" />
			<?} ?>
			<input type="button" id="btImprimir" value="Visualizar Impressão" onclick="imprimir('<?=$_GET['turid']; ?>');" />
			<input type="button" id="btVoltar" value="Voltar" onclick="javascript: window.location.href = 'siscap.php?modulo=principal/listaTurma&acao=A'" />
		</td>
	</tr>
</table>
<script>
function visualizarFicha(curid, alucpf){
	//window.open('siscap.php?modulo=principal/fichaInscricao&acao=A&alucpf='+alucpf+'&curid='+curid+'&pagina=popup','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
	window.open('siscap.php?modulo=principal/popupimprimirInscricao&acao=A&alucpf='+alucpf+'&curid='+curid+'&pagina=popup','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=750, width=900');
}
function imprimir(turid){
	window.open('siscap.php?modulo=relatorio/popupImprimirRelatorioTurma&acao=A&turid='+turid,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=450, width=1010');
}
</script>