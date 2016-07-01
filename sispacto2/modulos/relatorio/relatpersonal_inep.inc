<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Relatório do INEP</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

$sql = "
select distinct 
'2014' as ano_do_curso_de_formacao_1,
trim(replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')) as cpf_do_professor_2,
i.fk_cod_docente as cod_censo_inep_do_professor_3,
tpacodigoturma as cod_turma_censo_inep_4,
tpacodigoescola as cod_escola_censo_inep_5,
case when p.muncod is not null then 'Municipal'
     when p.estuf  is not null then 'Estadual' end as rede_6,
p.muncod as codigo_municipio_rede_7,
p.estuf  as codigo_estado_rede_8,
un.unisigla as ies_curso_9,
un.uniuf as codigo_ies_10,
CASE WHEN i2.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN trim(replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')) ELSE i2.iuscpf END as cpf_do_orientador_11,
i2.fk_cod_docente as cod_censo_inep_do_orientador_12,
CASE WHEN i2.iustipoorientador='tutoresproletramento' then 'Tutores Pró-Letramento'
     WHEN i2.iustipoorientador='tutoresredesemproletramento' then 'Professores da rede que não foram Tutores do Pró-Letramento'
     WHEN i2.iustipoorientador='profissionaismagisterio' then 'Profissionais do Magistério com experiência em formação de professores' END as tipo_orientador_13,
CASE WHEN i.iustipoprofessor='cpflivre' THEN 'Não bolsista'
     WHEN i.iustipoprofessor='censo' THEN 'Bolsista' END as situacao_bolsa_14,
CASE WHEN cer.cerfrequencia < 50 then 'DESISTIU' else 'NÃO DESISTIU' END as situacao_desistencia_15,
cer.cerfrequencia||'%' as situacao_frequencia_16,
CASE WHEN cer.cerfrequencia >= 75 then 'RECOMENDADO'
     ELSE 'NÃO RECOMENDADO' END as situacao_certificacao_17,
'[NÃO POSSUIMOS DADOS]' as situacao_premiacao_18
from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118 
left join sispacto2.turmasprofessoresalfabetizadores ta on ta.iusd = i.iusd and tpastatus='A' and tpaconfirmaregencia=true
left join sispacto2.pactoidadecerta p on p.picid = i.picid 
left join sispacto2.universidadecadastro unc on unc.uncid = i.uncid 
left join sispacto2.universidade un on un.uniid = unc.uniid 
left join sispacto2.orientadorturma ot on ot.iusd = i.iusd 
left join sispacto2.turmas tu on tu.turid = ot.turid 
left join sispacto2.identificacaousuario i2 on i2.iusd = tu.iusd 
left join sispacto2.certificacao cer on cer.iusd = i.iusd 
where i.iusstatus='A' order by 2";


ob_clean();
header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
header ( "Pragma: no-cache" );
header ( "Content-type: application/xls; name=rel_inep_".date("Ymdhis").".xls");
header ( "Content-Disposition: attachment; filename=rel_inep_".date("Ymdhis").".xls");
header ( "Content-Description: MID Gera excel" );

$dados = $db->carregar($sql);

$cabecalho = array_keys($dados);

$db->monta_lista_tabulado($dados,$cabecalho,100000000,5,'N','100%','');


		
		
?>