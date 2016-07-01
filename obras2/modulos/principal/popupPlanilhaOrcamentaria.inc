<?
include_once APPRAIZ . "par/classes/modelo/PreObra.class.inc";
include_once APPRAIZ . "par/classes/controle/SubacaoControle.class.inc";
verificaSessao('obra');


//echo cabecalhoObra(1006310);
//5227
monta_titulo( 'Planilha Orçamentária' , '');
$preid = $_GET['preid'];
if(!$preid)
    exit;
function prePegarDocid( $preid ) {

    global $db;

    $sql = "SELECT
				docid
			FROM
				obras.preobra
			WHERE
			 	preid = " . (integer) $preid;

    return (integer) $db->pegaUm( $sql );

}
function prePegarEstadoAtual( $docid ) {

    global $db;

    $sql = "SELECT
				ed.esdid
			FROM
				workflow.documento d
			INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
			WHERE
				d.docid = " . $docid;

    $estado = (integer) $db->pegaUm( $sql );

    return $estado;

}
function verificaMi( $preid = null ){
    global $db;

    if( !$preid ){
        return false;
    }

    $verifica = false;
    $categoria = $db->pegaUm("SELECT
							pto.ptocategoria
						FROM
							obras.preobra po
						INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
						WHERE
							po.preid = ".$preid );

    if( $categoria ){
        $verifica = true;
    }
    return $verifica;
}
function verificaTipoObra($preid)
{
    global $db;
    $sql = "SELECT
					ptoid
				FROM obras.preobra pre
				WHERE preid = '{$preid}'";

    return $preid ? $db->pegaUm($sql) : false;
}
function verificaTipoFundacao($preid)
{
    global $db;
    $sql = "SELECT
					pretipofundacao
				FROM obras.preobra pre
				WHERE preid = '{$preid}'";

    return $db->pegaUm($sql);
}


$reformulaMI = verificaMi( $preid );
$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);



function montaArvoreAberta( $preid){
    global $db;
    $_POST = array();

    $ptoid = verificaTipoObra($preid);
    $ptoid = $_POST['ptoid'] ? $_POST['ptoid'] : $ptoid;
	if(empty($ptoid)){
		return '';
	}

    $tipoFundacao = verificaTipoFundacao($preid);

    $where = $where2 = $whereBoFilhos = $sql2 = $boFiltro = "";

    $and = "";
    if($_POST['_tartarefa']){
        $where .= " t._tartarefa = {$_POST['_tartarefa']} ";
        $where2 .= " t._tartarefa = {$_POST['_tartarefa']} ";
        $whereBoFilhos .= " and t._tartarefa = {$_POST['_tartarefa']} ";
    }

    $arTipos = array(2, 7);
    if(!empty($tipoFundacao) && in_array($ptoid, $arTipos)){
        $stWhere .= " AND itctipofundacao = '{$tipoFundacao}' ";
    }

    $sql = "SELECT
				po.docid, po.estuf, pto.ptocategoria
			FROM
				obras.preobra po
			LEFT JOIN obras.pretipoobra pto on pto.ptoid = po.ptoid
			WHERE
			 	po.preid = " . (integer) $preid;

    $docid = $db->pegaLinha( $sql );

    if( !empty($docid['docid']) ){
        $sql = "SELECT
					ed.esdid
				FROM
					workflow.documento d
				INNER JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE
					d.docid = " . $docid['docid'];

        $esdid = (integer) $db->pegaUm( $sql );
    }

    $verifica = false;
    $categoria = $db->pegaUm("SELECT
								pto.ptocategoria
							FROM
								obras.preobra po
							INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid
							WHERE
								po.preid = ".$preid );

    if( $categoria ){
        $verifica = true;
    }

    if($verifica || $esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI){

        include_once(APPRAIZ."par/classes/WSSigarp.class.inc");
        $oWSSigarp = new WSSigarp();

        $atualizaMI = false;
        if( $_POST['atualmi'] ){
            $atualizaMI = true;
        }

        $arrayD = $itemCategoriaObra = $oWSSigarp->listarItemCategoria( $docid['ptocategoria'], $docid['estuf'],(integer) $preid, $atualizaMI );
//			$arrayD = $itemCategoriaObra = $oWSSigarp->solicitarItemObra();

        echo $arrayD;
        die;

    } else {

        $sql = "SELECT
						itc.itcid,
						itc.itcdescricao as itcdescricao,
						itc.itcordem,
						itc.itcstatus,
						itc.itcdtinclusao,
						itc.ptoid,
						itc.itcidpai,
						--coalesce(itc.itcvalorunitario::numeric, 0) as itcvalorunitario,
						itc.itcvalorunitario,
						--itcvalorunitario,
						coalesce(ppo.ppovalorunitario, 0) as ppovalorunitario,
						umd.umdeesc,
						ppo.ppoid,
						pre.preid,
						itc.itctipofundacao,
						itc.itccodigoitem,
						itc.itcidpai,
						itc.itccodigoitemcodigo,
						itc.itcquantidade
					FROM obras.preobra pre
					INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
					LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
					LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
					WHERE pre.ptoid = {$ptoid}
					AND itcstatus = 'A'
					AND pre.preid = '{$preid}'
					AND itccodigoitemcodigo <> ''
					{$stWhere}
					ORDER BY itccodigoitemcodigo
					--limit 1
					";
        $arDados = $db->carregar($sql);

        $sql = "select sum(total) from (
						SELECT
							--trim(to_char(coalesce(sum(itc.itcvalorunitario::numeric*ppo.ppovalorunitario), 0), '9999999990D99')) as total
							itc.itcvalorunitario::numeric*ppo.ppovalorunitario as total
						FROM obras.preobra pre
							INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
							LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
							LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
						WHERE pre.ptoid = {$ptoid}
						AND pre.preid = '{$preid}'
						AND itcstatus = 'A'
						{$stWhere}
					) as total
					";

        $total = $db->pegaUm($sql);

        $count = 0;
        $count2 = 0;

    }

    if(is_array($arDados) && $arDados[0] != "" ){
        $itens = array();
        $boCima = $boBaixo = false;
        foreach($arDados as $dados){
            //$img = $dados["taraberto"] == 't' ?  "menos.gif" : "mais.gif";
            $img = "menos.gif";
            /**
             * Verifica se tem Filho
             */
            $sql = "SELECT
							count(1)
						FROM obras.preobra pre
						INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
						WHERE pre.ptoid = {$ptoid}
							AND pre.preid = '{$preid}'
							AND itc.itcstatus = 'A'
							AND itc.itccodigoitemcodigo <> ''
							AND itc.itcidpai = {$dados['itcid']}";

            $boFilho = $db->pegaUm($sql);

            $nivelCorrente = (strlen($dados['itccodigoitemcodigo']) / 3);

            $arDados[$count]['boFilho'] 		 = $boFilho;
            $arDados[$count]['img'] 			 = $img;
            // se nao tiver quantidade, retorna 0. se tiver e for inteiro, retorna o inteiro (sem casas decimais). se tiver casas decimais, formato com duas casas e o divisor é ponto para manter compatibilidade
            $arDados[$count]['itcquantidade'] 	 = !$dados['itcquantidade'] ? 0 : ((is_int($dados['itcquantidade']))?$dados['itcquantidade']:number_format($dados['itcquantidade'],2,".",""));
            //$arDados[$count]['itccodigoitemcodigo'] = !$dados['itccodigoitemcodigo'] ? "0" : (int)$dados['itccodigoitemcodigo'];
            $arDados[$count]['itccodigoitemcodigo'] = $dados['itccodigoitemcodigo'];
            $arDados[$count]['itcdescricao'] 	 = utf8_encode($dados['itcdescricao']);
            $arDados[$count]['umdeesc'] 		 = ucfirst($dados['umdeesc']);
            $arDados[$count]['ppovalorunitario'] = ucfirst($dados['ppovalorunitario']);
            $arDados[$count]['total'] 			 = $total;

            $count++;
        }
        return simec_json_encode($arDados);
    } else {
        echo '';
    }
}

?>
<html>
<head>
    <title>Planilha Orçamentária</title>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
    <script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
</head>
<body topmargin="0" leftmargin="0">
        <table id="tabela_planilha" class="tabela" bgcolor="#f5f5f5" cellpadding="3" align="center">
            <tr style="background-color: #e0e0e0">
                <td style="font-weight:bold; text-align:center; width:20%;">Descriçao do item</td>
                <td style="font-weight:bold; text-align:center; width:10%;">Valor Unitario</td>
                <td style="font-weight:bold; text-align:center;">Unidade de Medida</td>
                <td style="font-weight:bold; text-align:center; width:30%;">Quantidade</td>
                <td style="font-weight:bold; text-align:center; width:10%;">Valor</td>
                <td style="font-weight:bold; text-align:right; width:10%;">%</td>
            </tr>
        </table>
    </body>
</html>

<script type="text/javascript">
    data = <?=montaArvoreAberta($preid);?>;
    var cor = "#f0f0f0";
    var total_valor_unitario = 0;
    var total_valor = 0;
    var arrItens = new Array();
    var num = 0;
    function montaLista(data) {

        var idNivel = new Array();
        var totalValor = 0;
        var totalValorUni = 0;

        jQuery.each(data, function (i, item) {
            var itcid = item.itcid;
            var itcidpai = item.itcidpai;
            var boFilho = item.boFilho;
            var img = item.img;
            var itccodigoitem = item.itccodigoitem;
            var itcdescricao = item.itcdescricao;
            var itccodigoitemcodigo = item.itccodigoitemcodigo;
            var itcquantidade = item.itcquantidade;
            var umdeesc = item.umdeesc;
            var ppovalorunitario = item.ppovalorunitario;
            var ppoid = item.ppoid;
            var tamanho = itccodigoitemcodigo.length;
            var nivel = tamanho / 3;

            //	alert(item.itccodigoitemcodigo+" - "+item.itcid+" - "+ppovalorunitario);

            if (umdeesc == null) {
                umdeesc = '-';
            }
            total_valor_unitario = ( (total_valor_unitario * 1) + (ppovalorunitario * 1) );
            total_valor = ( (total_valor * 1) + (ppovalorunitario * itcquantidade) );

            if (itcid) {
                arrItens[num] = new Object();
                arrItens[num].itcid = itcid;
                arrItens[num].valor_unitario = ppovalorunitario;
                arrItens[num].valor = (ppovalorunitario * itcquantidade);
                num++;
            }

            idNivel[nivel] = itcid;
            var id = '';
            // prepara para forma o id das TR
            for (i = 1; i <= nivel; i++) {
                id += (i == 1 ? idNivel[i] : '_' + idNivel[i]);
            }

            // Identação
            var espaco = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
            var espacoTemp = "";

            if (itcidpai) {
                for (y = 1; y < nivel; y++) {
                    espacoTemp = espacoTemp + espaco;
                }
            }

            var seta = "";
            if (espacoTemp) {
                seta = "<img src=\"../imagens/seta_filho.gif\">";
            }

            if (cor == "#fafafa") {
                cor = "#f0f0f0";
            } else {
                cor = "#fafafa";
            }

            var html = "<tr id=\"" + id + "\" style=\"background: " + cor + " \" cor=\"" + cor + "\">";
            if (boFilho > 0) {
                html += "<td>" + espacoTemp + seta
                + "<a href=\"#\" onclick=\"alteraIcone('" + id + "');\"><img id=\"img_" + id + "\" src=\"../imagens/" + img + "\" border=\"0\"></a> "
                + itccodigoitem + " " + itcdescricao + "</td>";
            } else {
                html += "<td>" + espacoTemp + seta + itccodigoitem + " "
                + itcdescricao + "</td>";
            }


            // Valor Unitário = ppovalorunitario
            var stAtivo = '<?=$stAtivo ?>';

            // Verifico se são obras MI
            <? if($esdid == 683 || $reformulaMI){?>
            if (boFilho < 1) {
                //html += "<td>"+parseFloat(ppovalorunitario).toFixed(2).replace(".",",");
                html += "<td>" + mascaraglobal('###.###.###.###,##', parseFloat(ppovalorunitario).toFixed(2));
                html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\"" + parseFloat(ppovalorunitario).toFixed(2).replace(".", ",") + "\">";
                html += "<input type=\"hidden\" name=\"itcid[]\" value=\"" + itcid + "\">";
                html += "<input type=\"hidden\" name=\"ppoid[]\" value=\"" + ppoid + "\"></td>";
            } else {
                html += "<td id=\"valor_unitario_pai_" + itcid + "\"></td>";
            }
            <? } else { ?>
            if (boFilho < 1 && (itcquantidade > 0)) {
                html += "<td><input type=\"text\" class=\"normal itcvalorunitario\" id=\"item_" + itcid + "\" " + stAtivo + "name=\"ppovalorunitario[]\" value=\"" + parseFloat(ppovalorunitario).toFixed(2).replace(".", ",") + "\">";
                html += "<input type=\"hidden\" name=\"ppovalorunitario_ant[]\" value=\"" + parseFloat(ppovalorunitario).toFixed(2).replace(".", ",") + "\">";
                html += "<input type=\"hidden\" name=\"itcid[]\" value=\"" + itcid + "\">";
                html += "<input type=\"hidden\" name=\"ppoid[]\" value=\"" + ppoid + "\"></td>";
            } else {
                html += "<td id=\"valor_unitario_pai_" + itcid + "\"></td>";
            }
            <? } ?>

            // Unidade de Medida
            html += "<td align=\"center\" id=\"umdeesc" + umdeesc + " \">" + umdeesc + "</td>";

            // Quantidade = itcquantidade
            // Verifico se são obras MI
            <? if($esdid == 683 || $reformulaMI){?>
            if (boFilho < 1) {
                html += "<td><input type=\"text\" class=\"normal quantidade\" onblur=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" onkeyup=\"this.value = mascaraglobal('###.###.###.###,##',this.value);\" size=\"17\" maxlength=\"17\" id=\"quantidade_" + itcid + " \" " + stAtivo + " name=\"itcquantidade[]\" value=\"" + itcquantidade + "\">";
                //html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_"+itcid+" \">"+itcquantidade+"</td>";
            } else {
                html += "<td id=\"qtde_pai_" + itcid + "\"></td>";
            }
            <? } else { ?>
            if (boFilho < 1) {
                html += "<td class=\"quantidade\" align=\"right\" id=\"quantidade_" + itcid + " \">" + itcquantidade + "</td>";
            } else {
                html += "<td id=\"qtde_pai_" + itcid + "\"></td>";
            }
            <? } ?>



            // Valor e porcentagem
            if (boFilho < 1) {
                html += "<td align=\"right\" id=\"valor_" + itcid + "\" class=\"valor\">" + (ppovalorunitario * itcquantidade).toFixed(2).replace(".", ",") + "</td>";
                html += "<td align=\"right\" id=\"porcentagem_" + itcid + "\"></td>";
            } else {
                html += "<td id=\"valor_pai_" + itcid + "\" ></td>";
                html += "<td id=\"percent_pai_" + itcid + "\" ></td>";
            }
            html += "</tr>";

            jQuery('#tabela_planilha tr:last').after(html);

        });
        //alert(totalValor)
        var html2 = "<tr style=\"background: #e0e0e0\" >";
        html2 += "<td><strong>TOTAL:</strong></td>";

        //html2 += "<td id=\"totalValorUni\" align=\"right\"><strong>" + parseFloat(total_valor_unitario).toFixed(2).replace(".",",") + "</strong></td>";
        html2 += "<td id=\"totalValorUni\" align=\"right\"></td>";

        html2 += "<td></td>";
        html2 += "<td></td>";
        html2 += "<td id=\"totalValor\" align=\"right\"><strong>" + parseFloat(total_valor).toFixed(2).replace(".",",") + "</strong></td>";
        html2 += "<td align=\"right\" ><strong>100</strong></td>";
        html2 += "</tr>";
        jQuery('#tabela_planilha tr:last').after(html2);

        for (i=0;i<=arrItens.length;i++){
            if(arrItens[i]){
                if(arrItens[i].valor == 0 && total_valor == 0){
                    var valor = 0;
                } else {
                    var valor = arrItens[i].valor / total_valor;
                }
                jQuery('#porcentagem_' + arrItens[i].itcid ).html(  parseFloat( valor * 100).toFixed(2).replace(".",",")  );
            }
        }

        jQuery('#aguarde_').hide();
    }
    $(function(){
        montaLista(data);

        jQuery('.itcvalorunitario').attr('disabled', 'disabled');
        jQuery('#tabela_planilha tr')
            .live('mouseover',function(){
                if(jQuery(this).attr('cor')){
                    jQuery(this).css('background','#ffffcc');
                }
            })
            .live('mouseout',function(){
                if(jQuery(this).attr('cor')){
                    jQuery(this).css('background',jQuery(this).attr('cor'));
                }
            });
    })

</script>