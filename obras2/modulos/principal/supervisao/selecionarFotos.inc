<?php
/**
 * Arquivo que seleciona as fotos da galeria para o Extrato da obra
 * 
 * @author Eduardo Dunice Neto
 * @since 21/06/2010
 * 
 */
$supvid = filter_input(INPUT_GET, 'supid', FILTER_SANITIZE_NUMBER_INT);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<?php
    $strSQL = "SELECT f.arqid, fotdsc, TO_CHAR(a.arqdata,'dd/mm/yyyy') AS aqodtinclusao 
            FROM obras2.supervisao s 
            INNER JOIN obras2.fotos f ON (f.supid = s.supid) 
            INNER JOIN public.arquivo a ON (a.arqid = f.arqid)
            WHERE s.supid = %d";
    $stmt = sprintf($strSQL, (int) $supvid);
    //echo $stmt; exit;
    $fotosVistorias = $db->carregar($stmt);

    if ($fotosVistorias) {

        echo '<tr>';
        $arqids = array();

        $j = 1;
        for ($i = 0; $i < count($fotosVistorias); $i++) {

            echo "<td align='center'>
            <img 
                src='../slideshow/slideshow/verimagem.php?_sisarquivo=obras2&newwidth=100&newheight=100&arqid={$fotosVistorias[$i]['arqid']}' 
                hspace='3' 
                vspace='3' 
                style='width:50px; 
                height:50px;'
            \>
            <br/>
            {$fotosVistorias[$i]['aqodtinclusao']}
            <br/>
            <input 
                type='checkbox' 
                name='fotos[]' 
                id='{$fotosVistorias[$i]['arqid']}' 
                value='{$fotosVistorias[$i]['arqid']}' 
                onclick='salvaFotosSelecionadas(this, {$fotosVistorias[$i]['arqid']})' 
            />
            </td>";

            if (is_int($j / 5)) {
                echo '</tr><tr>';
            }
            
            array_push($arqids, $fotosVistorias[$i]['arqid']);
            $j++;
        }

        echo "<td align='center'>
            <a data-target-flag='false' style='cursor:pointer;' onclick=\"selecionarFotos(this, [".implode(',', $arqids)."]);\">Selecinar Tudo</a>
        </td>";

        echo '</tr>';

    } else {
        echo '<tr><td align="center" style="color:#ee0000"> Não existem fotos cadastradas para esta obra. </td></tr>';
    }
?>
</table>
<script type="text/javascript">
function selecionarFotos(campo, arqids) {
    
    var dependent, flag;
    
    for (var i=0; i < arqids.length; i++) {
        
        dependent = document.getElementById(arqids[i]);
        
        if ($(campo).attr("data-target-flag") === "false") {
            
            dependent.checked=true;
            if (false === findArray(dependent.value, arrFs)) {
                arrFs.push(dependent.value);
            }
            flag="true";
            
        } else {
            (false!==findArray(dependent.value, arrFs)) && arrFs.splice(findArray(dependent.value, arrFs), 1);
            dependent.checked=false;
            flag="false";
        }
    }
    
    $(campo).attr("data-target-flag", flag);
    $("#fotoselecionadas").val(arrFs.join(","));
}
</script>

<?php die();?>