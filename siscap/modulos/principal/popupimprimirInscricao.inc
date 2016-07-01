<?php
require_once APPRAIZ . "www/includes/webservice/cpf.php";

$alucpf = $_GET['alucpf'];
$curid = ( !empty($_GET['curid']) ? $_GET['curid'] : $_SESSION['siscap']['curid']);

monta_titulo( 'Dados do Servidor', '' );

$sql = "SELECT alu.alucpf, alu.alunome, alu.aluareaatuacao, alu.aluendtrabalho, alu.alumatchefia,
  			alu.aluemail, alu.aluteletrabalho, alu.aluteleresidencial, alu.alutelecelular, alu.alugecoodequip, alu.alugecoodproj,
  			case when alu.alunivel = 'ME' then 'Médio' 
            	when alu.alunivel = 'SU' then 'Superior'
                when alu.alunivel = 'PG' then 'Pós Graduação'
                when alu.alunivel = 'MD' then 'Mestrado'
                when alu.alunivel = 'DO' then 'Doutorado'
            end as alunivel, 
            alu.alucurso, alu.aluarea, alu.alunomechefia, alu.aluemailchefia, alu.aluteletrabalhochefia, alu.alucargofuncaochefia,
  			alu.aluneceateespecial, alu.aluatendespecial, 
            case when ac.alcconhecimento = 'NE' then 'Nenhum' 
            	when ac.alcconhecimento = 'BA' then 'Básico'
                when ac.alcconhecimento = 'ID' then 'Intermediário'
                when ac.alcconhecimento = 'AV' then 'Avançado' 
            end as aluconhecitema, 
            case when ac.alcexperiencia = 'MU' then 'Muita' 
            	when ac.alcexperiencia = 'PO' then 'Pouca'
                when ac.alcexperiencia = 'NE' then 'Nenhuma'
            end as aluexpreltema, ac.alcexpectativas,
            usu.usudatanascimento, case when usu.ususexo = 'M' then 'Masculino' else 'Feminino' end ususexo, usu.usucpf,
            cs.nu_matricula_siape, cs.co_uorg_lotacao_servidor, cs.no_servidor, cs.co_funcao, cs.co_nivel_funcao, cs.ds_funcao, 
            cs.ds_situacao_servidor, cs.co_orgao_lotacao_servidor, cs.sg_unidade_organizacional, cs.ds_orgao, cs.ds_cargo_emprego,
            alu.alumatsiape, alu.aluvinculomec, alu.aluorgao, alu.alulotacao, alu.alucargo, alu.alufuncao
		FROM siscap.aluno alu
			left join seguranca.usuario usu on usu.usucpf = alu.alucpf
			left join (SELECT nu_matricula_siape, co_uorg_lotacao_servidor, no_servidor,
						  co_funcao, co_nivel_funcao, ds_funcao, ds_situacao_servidor, 
						  co_orgao_lotacao_servidor, sg_unidade_organizacional,
						  ds_orgao, ds_cargo_emprego, nu_cpf
						FROM siscap.tb_cadastro_servidor) cs on cs.nu_cpf = usu.usucpf
			inner join siscap.aluno_curso ac on ac.alucpf = alu.alucpf and ac.curid = $curid
		WHERE alu.alucpf = '".$alucpf."'";

$arDados = $db->pegaLinha( $sql );
$arDados = $arDados ? $arDados : array();

extract( $arDados );
$aluemailconf = $aluemail;
$aluemailchefiaconf = $aluemailchefia;
$alumatchefia = trim($alumatchefia);

$nu_matricula_siape = (empty($nu_matricula_siape) ? $alumatsiape : $nu_matricula_siape);
$ds_situacao_servidor = (empty($ds_situacao_servidor) ? $aluvinculomec : $ds_situacao_servidor);
$sg_unidade_organizacional = (empty($sg_unidade_organizacional) ? $alulotacao : $sg_unidade_organizacional);
$ds_orgao = (empty($ds_orgao) ? $aluorgao : $ds_orgao);
$ds_cargo_emprego = (empty($ds_cargo_emprego) ? $alucargo : $ds_cargo_emprego);
$ds_funcao = (empty($ds_funcao) ? $alufuncao : $ds_funcao.$co_nivel_funcao);

$aluteletrabalho = "(" . substr( $aluteletrabalho, 0, 0) . substr( $aluteletrabalho, 0, 2).") " . substr( $aluteletrabalho, 2, 4) . "-" . substr( $aluteletrabalho, 6);
$aluteleresidencial = "(" . substr( $aluteleresidencial, 0, 0) . substr( $aluteleresidencial, 0, 2).") " . substr( $aluteleresidencial, 2, 4) . "-" . substr( $aluteleresidencial, 6);
$alutelecelular = "(" . substr( $alutelecelular, 0, 0) . substr( $alutelecelular, 0, 2).") " . substr( $alutelecelular, 2, 4) . "-" . substr( $alutelecelular, 6);
$aluteletrabalhochefia = "(" . substr( $aluteletrabalhochefia, 0, 0) . substr( $aluteletrabalhochefia, 0, 2).") " . substr( $aluteletrabalhochefia, 2, 4) . "-" . substr( $aluteletrabalhochefia, 6);

if( empty($aluatendespecial) ){
	$display = "style=\"display: none\";";
} else {
	$display = "style=\"display: ''\";";
}

if(empty($alumatchefia)) unset($_SESSION['siscap']['tipo']);

$tipo = $_SESSION['siscap']['tipo'];
?>
<style type="">
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="usucpf" id="usucpf" value="<?=$usucpf; ?>">
	<input type="hidden" name="alucpf" id="alucpf" value="<?=$alucpf; ?>">
	
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<th colspan="2">Ficha de Inscrição</th>
		</tr>
		<tr>
			<td width="15%" class="subtitulodireita">CPF:</td>
			<td colspan="6" width="35%"><?=formatar_cpf($alucpf); ?></td>
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
			<td><?=$nu_matricula_siape; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Vínculo com o MEC:</td>
			<td><div id="vinculo"><?=$ds_situacao_servidor; ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Orgão:</td>
			<td><div id="lotacao"><?=$ds_orgao; ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Lotação:</td>
			<td><div id="unidade"><?=$sg_unidade_organizacional; ?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cargo:</td>
			<td><div id="cargo"><?=$ds_cargo_emprego;?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Função:</td>
			<td><div id="funcao"><?=$ds_funcao;?></div></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área de Atuação:</td>
			<td><?=$aluareaatuacao; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Endereço do Trabalho:</td>
			<td><?=$aluendtrabalho; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">E-mail Instituicional:</td>
			<td><?=$aluemail; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Telefone do Trabalho:</td>
			<td><?=$aluteletrabalho; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Residêncial:</td>
			<td><?=$aluteleresidencial; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Celular:</td>
			<td><?=$alutelecelular; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Gerencia ou coordena equipes?:</td>
			<td><?=($alugecoodequip == 'S' ? 'Sim' : 'Não'); ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Gerencia ou coordena projetos?:</td>
			<td><?=($alugecoodproj == 'S' ? 'Sim' : 'Não'); ?></td>
		</tr>
		<tr>
			<th colspan="2">Escolaridade</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Nível:</td>
			<td><?=$alunivel; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Curso:</td>
			<td><?=$alucurso; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Área:</td>
			<td><?=$aluarea; ?></td>
		</tr>
		<?if( $tipo != 'relatorio' && !empty($alumatchefia) ){ ?>
		<tr>
			<th colspan="2">Informações sobre a Chefia Imediata</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Matrícula:</td>
			<td><?=$alumatchefia; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome Completo:</td>
			<td><?=$alunomechefia; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">E-mail:</td>
			<td><?=$aluemailchefia; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Telefone de Trabalho:</td>
			<td><?=$aluteletrabalhochefia; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Cargo / Função:</td>
			<td><?=$alucargofuncaochefia; ?></td>
		</tr>
		<tr>
			<th colspan="2">Atendimento Especial</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Necessita de atendimento especial durante o período de realização do curso?</td>
			<td><?=($aluneceateespecial == 'S' ? 'Necessito de atendimento pessoal' : 'Não necessito de atendimento pessoal'); ?></td>
		</tr>
		<tr id="especial" <?=$display; ?>>
			<td class="subtitulodireita">Este espaço é destinado a descrição das necessidades especiais durante o período de realização do curso:</td>
			<td><?=$aluatendespecial; ?></td>
		</tr>
		<tr>
			<th colspan="2">Conhecimento, experiências e expectativas em relação ao curso/evento</th>
		</tr>
		<tr>
			<td class="subtitulodireita">Qual é o seu conhecimento sobre o tema da capacitação?:</td>
			<td><?=$aluconhecitema; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Qual é o sua experiência relacionada ao tema?:</td>
			<td><?=$aluexpreltema; ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Este espaço é destinado a descrição de sua experiência em relação ao curso que você pretende participar:</td>
			<td><?=$alcexpectativas; ?></td>
		</tr>
		<?}
		if( $_GET['pagina'] != 'popup' ){ ?>
		<tr>
			<th colspan="2">Termo de Compromisso</th>
		</tr>
		<tr>
			<td colspan="2"><input type="checkbox" name="checkdeclaro" id="checkdeclaro"> Declaro estar ciente dos termos das normas regulamentares da Portaria/MEC nº1.507, de 28 de outubro de 2010</td>
		</tr>
		<?} ?>
		<tr bgcolor="#D0D0D0">
			<td colspan="6" style="text-align: center">					
				<input type="button" id="btImprimir" value="Imprimir" onclick="javascript: window.print();" />
				<input type="button" id="btFecharr" value="Fechar" onclick="javascript: window.close();" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript">
jQuery.noConflict();

jQuery(document).ready(function(){
	var comp  = new dCPF();
	comp.buscarDados( $('usucpf').getValue() );
	if (comp.dados.no_pessoa_rf != ''){
		jQuery('#nome').html( comp.dados.no_pessoa_rf );
		var dataNasc = comp.dados.dt_nascimento_rf.substr(6,2) +'/'+ comp.dados.dt_nascimento_rf.substr(4,2) +'/'+ comp.dados.dt_nascimento_rf.substr(0,4);

		jQuery('#datanasc').html( dataNasc );
		if( comp.dados.sg_sexo_rf == 'M' ) jQuery('#sexo').html('Masculino');
		if( comp.dados.sg_sexo_rf == 'F' ) jQuery('#sexo').html('Feminino');
	}
});
</script>