<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<?php
global $db;

if( ! $_REQUEST['preid'])
{
	echo '<script>
						alert("Faltam dados.");
						window.close();
				</script>';
	die;
}


#Montar Titulo
monta_titulo('Histórico de Prorrogação de Prazo', '');

$sqlPreobra = "SELECT pre.preid, predescricao FROM obras.preobra pre WHERE pre.preid = {$_REQUEST['preid']}";
$arrPreobra = $db->pegaLinha($sqlPreobra);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <colgroup>
        <col width="20%" />
        <col width="80%" />
    </colgroup>
    <tr>
        <td class="SubTituloDireita">Obra:</td>
        <td><?php echo $arrPreobra['preid']. ' - '. $arrPreobra['predescricao'] ?></td>
    </tr>
</table>
<?php
$sql = "SELECT 
            '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer\" class=\"historicoProrrogacao\" id=\"'|| arqid ||'\" />' as acao,
            to_char(popdata, 'DD/MM/YYYY') as dtInicial,
            to_char(popdataprazo, 'DD/MM/YYYY') as dtSolicitada,
            to_char(popdataprazoaprovado, 'DD/MM/YYYY') as dtAprovada,
            to_char(popdatavalidacao, 'DD/MM/YYYY') as dtValidacao,
            CASE
                WHEN popstatus = 'A' THEN 'Ativo'
                WHEN popstatus = 'P' THEN 'Pendente'
                WHEN popstatus = 'C' THEN 'Cancelado'
                WHEN popstatus = 'I' THEN 'Inativo'
                WHEN popstatus = 'F' THEN 'Recusado'
            END
        FROM obras.preobraprorrogacao 
        WHERE preid = {$_REQUEST['preid']} 
        ORDER BY popid";
$cabecalho = array("Ação", "Data Inicio", "Data Solicitada", "Data Aprovada", "Data Validação", "Status");
$db->monta_lista($sql, $cabecalho, 50, 5, 'N', '100%');
?>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<script type="text/javascript">
    jQuery('body').on('click', '.historicoProrrogacao', function () {
        return window.open('par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&arqidProrrogacao=' + this.id,
            'Consulta prorrogação',
            "height=200,width=200,scrollbars=yes,top=50,left=200").focus();
    });
</script>