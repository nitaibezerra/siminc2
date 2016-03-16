<?php
set_time_limit(0);
ini_set('memory_limit', '1024M');

monta_titulo( 'Relatórios de Turmas', '');
?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<?
if( !empty($_GET['turid']) ){
	$filtro = " and alt.turid = ".$_GET['turid'];
}

$sql = "SELECT t.turturma, c.curdsc FROM siscap.turma t
	inner join siscap.curso c on c.curid = t.curid
WHERE
	t.turid = ".$_GET['turid'];

$arTurma = $db->pegaLinha( $sql );

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
</table>
<?

$sql = "SELECT DISTINCT alu.alunome, substr(alu.alucpf, 0, 4)||'.'||substr(alu.alucpf, 4, 3)||'.'||substr(alu.alucpf, 7, 3)||
		    '-'||substr(alu.alucpf, 10, 2) as cpf, 
		    	case when cse.nu_matricula_siape is null then alu.alumatsiape else cse.nu_matricula_siape end as matricula,
		    case when cse.sg_unidade_organizacional is null then alu.alulotacao else cse.sg_unidade_organizacional end as lotacao, 
		    case when cse.ds_situacao_servidor is null then alu.aluvinculomec else cse.ds_situacao_servidor end as situacao,
			case when cse.ds_cargo_emprego is null then alu.alucargo else cse.ds_cargo_emprego end as cargos, 
		    case when cse.ds_funcao is null then alu.alufuncao else cse.ds_funcao||co_nivel_funcao end as funcao, 
		    to_char(alt.atudatainscricao, 'DD/MM/YYYY HH24:MI:SS') as data, 
		    case when alt.atustatus = 'P' then 'Pendente' 
			when alt.atustatus = 'A' then 'Autorizado'
			when alt.atustatus = 'N' then 'Não Autorizado' end as autorizacao
		FROM siscap.aluno alu
		    inner join siscap.aluno_turma alt on alt.alucpf = alu.alucpf
		    inner join siscap.turma tur on tur.turid = alt.turid
            left join (SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor,
                        co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor, 
                        co_orgao_lotacao_servidor, sg_unidade_organizacional,
                        ds_orgao, ds_cargo_emprego, nu_cpf
                      FROM siscap.tb_cadastro_servidor) cse on cse.nu_cpf = alu.alucpf
		WHERE
			alt.atusituacao = 'A'
			$filtro";

$cabecalho = array("Nome", "CPF", "SIAPE", "Lotação", "Sit. Funcional", "Cargo", "Função", "Inscrição", "Autorização");
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
?>
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="6" style="text-align: center" class="div_rolagem">
			<input type="button" id="btImprimir" value="Imprimir" onclick="javascript: window.print();" />
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>