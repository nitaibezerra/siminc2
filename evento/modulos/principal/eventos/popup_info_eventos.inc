<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    $tipo  = $_REQUEST['tipo'];
    
    if( $tipo == 'emp' ){
        $texto = "EMPENHO";
        $emuid = $_REQUEST['emuid'];
        $AND = "AND emp.emuid = {$emuid}";
    }
    
    if( $tipo == 'cont' ){
        $texto = "CONTRATO";
        $coeid = $_REQUEST['coeid'];
        $AND = "AND emp.coeid = {$coeid}";
    }
    
    if( $tipo == 'und' ){
        $texto = "UNIDADE DE RECURSO";
        $ureid = $_REQUEST['ureid'];
        $AND = "AND emp.ureid = {$ureid}";
    }
?>

<html>
    <title> Eventos Realizados por esse(a) <?=$texto;?> </title>
    <head>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">
        function abrirPopupInstituicao() {
            window.open('gestaodocumentos.php?modulo=principal/popupInstituicoes&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }

        function abrirPopupOrgao() {
            var tpsid = $('#tpsid').val();
            var array = tpsid.split('@');
            tpsid = trim(array[0].replace('tpsid_', ''));
            window.open('gestaodocumentos.php?modulo=principal/popupOrgao&acao=A&tpsid='+tpsid, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }

        function abrirPopupMantenedora() {
            window.open('gestaodocumentos.php?modulo=principal/popupMantenedora&acao=A', '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=820,height=560');
        }
    </script>
    </head>

    <br>

    <form name="formulario" id="formulario" action="" method="POST">
       	<input type="hidden" name="requisicao" id="requisicao" value="" />
        <input type="hidden" name="solid" id="solid" value="" />
        <input type="hidden" name="tipo_filtro" id="tipo_filtro" value="" />
        <input type="hidden" name="tarid" id="tarid" value="<?PHP echo $_GET['tarid']; ?>" />
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <tr>
                <td class="subtitulodireita" colspan="2"><center> <h3>Eventos realizados com esse(a) <?=$texto;?></h3></center></td>
            </tr>
        </table>
    </form>

    <br>

<?PHP
    $sql = "
        SELECT 	'Nº: '||ev.eveid as eveid,
                coenumcontrato,
                und.ungabrev, 
                empnumero||' - '||empnumeropi||'<br>'||empdescricao AS emponho,
                evetitulo,
                to_char(evedatainicio, 'DD/MM/YYYY') AS evedatainicio,
                to_char(evedatafim, 'DD/MM/YYYY') AS evedatafim,
                evecustoprevisto,
                osecustofinal,
                dpavalor,
                'Nº: '||osenumeroos AS osenumeroos,
                to_char(osedataemissaoos, 'DD/MM/YYYY') as osedataemissaoos,
                ev.estuf||' - '||mundescricao,                
                es.esddsc
	
        FROM evento.evento AS ev
        
        JOIN territorios.municipio AS m ON m.muncod = ev.muncod
        JOIN workflow.documento AS d ON d.docid = ev.docid 
        JOIN workflow.estadodocumento AS es ON es.esdid = d.esdid
        
        LEFT JOIN evento.ordemservico AS o ON o.eveid = ev.eveid
        LEFT JOIN evento.documentopagamento AS dp ON dp.eveid = ev.eveid
        
        JOIN evento.empenho_unidade AS emp ON emp.emuid = ev.emuid
        JOIN evento.unidaderecurso AS ure ON ure.ureid = emp.ureid
        JOIN public.unidadegestora AS und ON und.ungcod = ure.ungcod
        JOIN evento.contratopregao AS cont ON cont.coeid = emp.coeid 
        
        WHERE evestatus = 'A' {$AND}
    ";
    $cabecalho = array("Nº do Evento", "Contrato", "Unidade", "Empenho", "Evento", "Dt inicio", "Dt Fim", "Custo Infra", "Custo OS", "Custo PG", "Nº OS", "Data da OS", "UF/Município", "Status");
    $alinhamento = Array('center', '', '', '', '', 'right','right', 'right', 'center', 'center');
    //$tamanho = Array('5%', '7%', '5%', '20%', '40%', '5%', '5%', '7%', '7%','7%', '7%', '15%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'S', 'center', 'S', '', $tamanho, $alinhamento);
?>