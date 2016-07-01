<?PHP

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Mais Médicos - MEC', 'Relatório de Inscrições (Mantenedora)');

$abacod_tela = '';
$url = '';
$parametros = '';

//$db->cria_aba($abacod_tela, $url, $parametros);


$perfil = pegaPerfilGeral();
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">


    function pesquisarServidor(param) {
        if (trim(param) == 'fil') {
        	$('#submite').val('OK');
        	$('#tipoxls').val('');
            $('#formulario').submit();
        } else {
           location.href = 'maismedicomec.php?modulo=relatorio/relatorio_mantenedora_inscricoes&acao=A';
        }
    }
    
    function exportarXls() {
        $('#submite').val('OK');
        $('#tipoxls').val('OK');
        $('#formulario').submit();
    }

</script>


<form action="" method="POST" id="formulario" name="formulario">

    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="submite" name="submite" value=""/>
    <input type="hidden" id="tipoxls" name="tipoxls" value=""/>
	
	<?if(!in_array(PERFIL_IES, $perfil)){?>
	
	    <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

			<tr>
				<td width="40%" class ="SubTituloDireita"> Município: </td>
				<td>
					<?PHP
					$mantida_muncod = $_REQUEST['mantida_muncod'];
					$sql = "
                            SELECT  mun.muncod AS codigo,
                                    mundescricao || ' - ' || estuf AS descricao
                            FROM territorios.municipio mun
                            JOIN maismedicomec.municipioies munies on substr(mun.muncod, 1, 6) = munies.muncod
                            ORDER BY descricao
                        ";
					$db->monta_combo('mantida_muncod', $sql, 'S', "TODOS", '', '', '', 300, 'S', 'mantida_muncod', '', $mantida_muncod, 'Município', '', 'chosen-select');
					?>
				</td>
			</tr>
			<tr>
				<td width="40%" class ="SubTituloDireita"> Mantida: </td>
				<td>
					<?PHP
					$mantida = $_REQUEST['mantida'];
					$sql = "
                            SELECT  '1' AS codigo,
                                    'Indicada' AS descricao
                            union
                            SELECT  '2' AS codigo,
                                    'Candidata' AS descricao
                        ";
					$db->monta_combo('mantida', $sql, 'S', "TODOS", '', '', '', 300, 'S', 'mantida', '', $mantida, 'Mantida', '', 'chosen-select');
					?>
				</td>
			</tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="2">
	                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
	                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
	                <input type="button" name="expostarxls" value="Exportar XLS" onclick="exportarXls();" >
	            </td>
	        </tr>
	    </table>
	    
	 <?}?>
	 
</form>


<br>

<?PHP

if($_POST['submite']=='OK'){
	//filtro
	if($_REQUEST['mantida_muncod']){
		$and .= " and mm.muncod = '{$_REQUEST['mantida_muncod']}' ";
	}
	if($_REQUEST['mantida'] == '1'){
		$and1 .= " and mtddsc is null ";
	}
	if($_REQUEST['mantida'] == '2'){
		$and2 .= " and mtddsc is null ";
	}
}




$sql = "
        SELECT  mundescricao || ' - ' || mun.estuf as municipio,
        		mnmid || '/2015' as mnmid,
                mnmprioridade,
                mntrazaosocial,
                mtddsc as inscrita,
                '-' as candidata,
                CASE WHEN doc.docid is null THEN
                		'Não iniciado'
                	 ELSE
                	 	(select esddsc from workflow.estadodocumento where esdid = doc.esdid)
                END as situacao
        FROM maismedicomec.mantida AS m

        JOIN maismedicomec.mantidamunicipio AS mm ON mm.mtdid = m.mtdid

        JOIN maismedicomec.mantenedora AS mm2 ON mm2.mntid = mm.mntid

        JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = mm2.mntid and ur.rpustatus='A'

        LEFT JOIN workflow.documento AS doc ON doc.docid = mm2.docid

        left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod

        WHERE mtdstatus = 'A' $and $and2

        UNION

    	SELECT  mundescricao || ' - ' || mun.estuf as municipio,
    		    mnmid || '/2015' as mnmid,
                mnmprioridade,
                mntrazaosocial,
                '-' as inscrita,
                mtddsc as candidata,
                CASE WHEN doc.docid is null THEN
                		'Não iniciado'
                	 ELSE
                	 	(select esddsc from workflow.estadodocumento where esdid = doc.esdid)
                END as situacao
        FROM maismedicomec.mantidacandidata AS m

        JOIN maismedicomec.mantidamunicipiocandidata AS mm ON mm.mtdid = m.mtdid

        JOIN maismedicomec.mantenedora AS mm2 ON mm2.mntid = mm.mntid

        JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = mm2.mntid and ur.rpustatus='A'

        LEFT JOIN workflow.documento AS doc ON doc.docid = mm2.docid

        left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod

        WHERE mtdstatus = 'A' $and $and1

    	ORDER BY 1
    	";

//ver($sql,d);

$cabecalho = array("Município", "Nº de Inscrição", "Prioridade", "Mantenedora", "Indicada", "Candidata", "Situação");
$alinhamento = Array('','','','','','','','','','','');
$tamanho = Array('','','','','','','','','','','');


if($_POST['tipoxls']=='OK'){
	ob_clean();
	header("Content-type: application/vnd.ms-excel");   
	header("Content-type: application/force-download");  
	header("Content-Disposition: attachment; filename=simec_maismedicosmec_mantida.xls");
	header("Pragma: no-cache");
	$db->monta_lista($sql, $cabecalho, 500, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}else{
	$db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}
?>