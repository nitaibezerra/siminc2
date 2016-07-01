<?php 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:importatudo_sidor.inc
   Finalidade: permitir importar os dados do SIGPLAN fase quantitativa
   */
include APPRAIZ.'includes/class.ADODB_XML.inc';
$adodbXML = new adodb_xml("1.0","utf-8");

print 'regiao<br>';
@$adodbXML->ImportaXML($clientedb, "CargaRegiao_SIDOR.xml", "regiao",'REG','ppa');
print 'tipoacao<br>';

@$adodbXML->ImportaXML($clientedb, "CargaTipoAcao_SIDOR.xml", "tipoacao",'TAC','ppa');
print 'tipoorgao<br>';

@$adodbXML->ImportaXML($clientedb, "CargaTipoOrgao_SIDOR.xml", "tipoorgao",'TPO','ppa');
print 'tipoprograma<br>';

@$adodbXML->ImportaXML($clientedb, "CargaTipoPrograma_SIDOR.xml", "tipoprograma",'TPR','ppa');
print 'Unidade de medida<br>';

print 'orgao<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAOrgao_SIDOR.xml", "orgao",'ORG','ppa');
print 'natureza<br>';
$adodbXML->ImportaXML($clientedb, "CargaNatureza_SIDOR.xml", "natureza",'NAT','ppa');

print 'Municipio<br>';
$adodbXML->ImportaXML($clientedb, "CargaMunicipio_SIDOR.xml", "municipio",'mun','ppa')
;
print 'fonteppa<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAFonte_SIDOR.xml", "fonteppa",'FPP','ppa');
print 'esfera<br>';
$adodbXML->ImportaXML($clientedb, "CargaEsfera_SIDOR.xml", "esfera",'ESF','ppa');
print 'tipoatualizacao<br>';
$adodbXML->ImportaXML($clientedb, "CargaTipoAtualizacao_SIDOR.xml", "tipoatualizacao",'TAT','ppa');
print 'ppafuncao<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAFuncao_SIDOR.xml", "ppafuncao",'FUN','ppa');
print 'ppasubfuncao<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPASubFuncao_SIDOR.xml", "ppasubfuncao",'SFU','ppa');

print 'tipoinclusao<br>';
$adodbXML->ImportaXML($clientedb, "CargaTipoInclusaoAcao_SIDOR.xml", "tipoinclusaoacao",'TIN','ppa');
print 'orgaosiorg<br>';
$adodbXML->ImportaXML($clientedb, "CargaOrgaoSIORG_SIDOR.xml", "orgaosiorg",'OSI','ppa');

print 'tipodespesa<br>';

//$sql='update orgaosiorg set orgid=o.orgid from orgao o where o.orgcod=orgaosiorg.orgcod and o.organo=orgaosiorg.organo and orgaosiorg.orgid is null';
////$saida = $db->executar($sql);
//$db -> commit();

$adodbXML->ImportaXML($clientedb, "CargaTipoDespesa_SIDOR.xml", "tipodespesa",'TDP','ppa');
print 'produto<br>';
$adodbXML->ImportaXML($clientedb, "CargaProduto_SIDOR.xml", "produto",'PRO','ppa');
print 'Unidade<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAUnidade_SIDOR.xml", "unidade",'UNI','ppa');

print 'programa<br>';
$adodbXML->ImportaXML($clientedb, "CargaPPAPrograma_SIDOR.xml", "elabrev.ppaprograma_orcamento",'PRG','ppa');
print 'indicador<br>';

$adodbXML->upgradedb_ppa_programa(dbconnection, "CargaPPAIndicador_SIDOR.xml", "elabrev.ppaindicador_orcamento",'IND','ppa');
//$sql='update programa set prgid=o.orgid from orgao o where o.orgcod=programa.orgcod and o.organo=programa.prgano and programa.orgid is null';
//$saida = $db->executar($sql);
//$db -> commit();

?>
