<?
$perfis = pegaPerfilGeral();

if($_REQUEST['uninome'] || $_REQUEST['unisigla'] || $_REQUEST['curdesc'] || $_REQUEST['coordid']) {
	
	$where[] = "s.sifexecucaosisfor=true";
	$where[] = " s.sifvigenciadtini is not null ";
	$where[] = " s.sifvigenciadtfim is not null ";
	
	if ($_REQUEST['uninome']) {
	    $uninome = trim($_REQUEST['uninome']);
	    $where[] = "  u.unidsc ilike ('%{$uninome}%') ";
	}
	
	if ($_REQUEST['unisigla']) {
	    $unisigla = trim($_REQUEST['unisigla']);
	    $where[] = "u.uniabrev ilike ('%{$unisigla}%')";
	}
	
	if ($_REQUEST['curdesc']) {
	    $curdesc = trim($_REQUEST['curdesc']);
	    $where[] = " (cur.curid||' '||cur.curdesc ilike ('%{$curdesc}%') OR cur2.curid||' '||cur2.curdesc ilike ('%{$curdesc}%') OR oc.ocunome ilike ('%{$curdesc}%'))";
	}
	
	if($_REQUEST['coordid']) {
		$where[] = "(cor.coordid='".$_REQUEST['coordid']."' OR cor2.coordid='".$_REQUEST['coordid']."' OR cor3.coordid='".$_REQUEST['coordid']."')";
	}
	
	if (in_array(PFL_EQUIPE_MEC, $perfis)) {
	    $coordids = $db->carregarColuna("SELECT coordid FROM sisfor.usuarioresponsabilidade WHERE usucpf='" . $_SESSION['usucpf'] . "' AND pflcod='" . PFL_EQUIPE_MEC . "' AND rpustatus='A'");
	
	    if ($coordids) {
	        $where[] = "(cor.coordid IN('" . implode("','", $coordids) . "') OR cor2.coordid IN('" . implode("','", $coordids) . "') OR cor3.coordid IN('" . implode("','", $coordids) . "'))";
	    }
	}

}



$sql = "select u.uniabrev ||' - '|| unidsc as uninome,
       			case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     when s.ocuid is not null then oc.ocunome end as curdesc,
               s.sifid, s.sifvigenciadtini, s.sifvigenciadtfim
        from sisfor.sisfor s
                inner join public.unidade u on u.unicod = s.unicod and u.unitpocod = s.unitpocod 
		inner join workflow.documento doc on doc.docid = s.docidprojeto and doc.esdid = '" . ESD_PROJETO_VALIDADO . "'";


$sql .= "left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
				left join catalogocurso2014.curso cur on cur.curid = ieo.curid AND cur.curstatus = 'A'
				left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
				left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
				left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid AND cur2.curstatus = 'A' 
				left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
				left join seguranca.usuario usu on usu.usucpf = s.usucpf 
				left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
				left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
        where " . (($where)?implode(' AND ', $where):'1=2') . "
        order by 2, uninome
        ;";

$universidadecadastro = $db->carregar($sql);

?>

<script type="text/javascript" src="/estrutura/js/funcoes.js"></script>
<script>

<? if (in_array(PFL_CONSULTAMEC, $perfis)) : ?>
        jQuery(document).ready(function() {
            jQuery('#picid').attr('disabled', 'disabled');
            jQuery('#carregarOrientadoresSIS').css('display', 'none');
            jQuery('#salvaconfiguracoes').css('display', 'none');
        });
<? endif; ?>


    function carregarOrientadoresSIS() {
        if (document.getElementById('picid').value == '') {
            alert('Selecione um município/estado');
            return false;
        }

        var conf = confirm('Deseja realmente carregar este Município/Estado com Orientadores de Estudo SIS?');
        if (conf) {
            window.location = 'sispacto.php?modulo=principal/mec/mec&acao=A&requisicao=carregarOrientadoresSISPorMunicipio&picid=' + document.getElementById('picid').value;
        }
    }


    function selecionarLinhaPeriodoExecucao(obj) {
        obj.parentNode.parentNode.cells[0].childNodes[0].checked = true;
    }
</script>

<form method=post name="periodoreferencia" id="periodoreferencia">
    <input type="hidden" name="filtrarCurso" value="filtrar">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
        <tr>
            <td class="SubTituloDireita">Nome da Universidade</td>
            <td><?php echo campo_texto('uninome', "N", "S", "uninome", 60, 150, "", "", '', '', 0, '', '', $_REQUEST['uninome']); ?></td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Sigla da Universidade</td>
            <td><?php echo campo_texto('unisigla', "N", "S", "unisigla", 60, 150, "", "", '', '', 0, '', '', $_REQUEST['unisigla']); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Curso</td>
            <td><?php echo campo_texto('curdesc', "N", "S", "curdesc", 60, 150, "", "", '', '', 0, '', '', $_REQUEST['curdesc']); ?></td>
        </tr>
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
			<?php 
			$sql = "SELECT coordid as codigo, coordsigla as descricao FROM catalogocurso2014.coordenacao WHERE coorano='".$_SESSION['exercicio']."' ORDER BY coordid";
			$db->monta_combo('coordid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'coordid', '', $_REQUEST['coordid']);
			?>
			</td>
		</tr>

        <tr>
            <td align="center" bgcolor="#CCCCCC" colspan="2">
                <input type="submit" class="botao_enviar"  name="salvar" value="Pesquisar">
            </td>
        </tr>
    </table>
</form>

<? if($where) : ?>

<form method=post name="periodoreferencia" id="periodoreferencia">
    <input type="hidden" name="requisicao" value="cadastrarPeriodoReferencia">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" width="10%">Orientações</td>
            <td colspan="7">
                Definindo períodos de referência de cada universidade, esta definição ocorrerá quando a universidade for validada e tiver o Registro de Frequência fechado.<br>
                <span style="width:10px;background-color:#BFEFFF">&nbsp;&nbsp;&nbsp;</span> Cursos que não definiram o período de execução para pagamento de bolsas<br>
                <span style="width:10px;background-color:#FF6A6A">&nbsp;&nbsp;&nbsp;</span> Cursos que definiram o período de execução para pagamento de bolsas (ja iniciaram a execução)
            </td>
        </tr>
        <tr>
            <td colspan="8"><input style="margin-right: 5px;" type="checkbox" name="marcar_todos"  class="marcar-todos" marcar="check_cursos"/> Marcar Todos</td>
        </tr>

<? if ($universidadecadastro[0]) : ?>
            <?
            $auxCurso = "";
            foreach ($universidadecadastro as $uc) :
                $sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sisfor.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer";
                $sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sisfor.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";


                unset($arrini, $arrfim);

                // recuperando informações salvas
                $arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim
                                  FROM sisfor.folhapagamentoprojeto fu
                                  INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
                                  WHERE fu.sifid='" . $uc['sifid'] . "'");

                // recuperando informações salvas
                $arrCur = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoini, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofim
                                  FROM sisfor.folhapagamentocursista fu
                                  INNER JOIN sisfor.folhapagamento f ON f.fpbid = fu.fpbid
                                  WHERE fu.sifid='" . $uc['sifid'] . "'");

                $sifvigenciadtini = $uc['sifvigenciadtini'] ? explode("-", $uc['sifvigenciadtini']) : null;
                $sifvigenciadtfim = $uc['sifvigenciadtfim'] ? explode("-", $uc['sifvigenciadtfim']) : null;

                // Bolsista
                $arrini = explode("-", $arr['mesanoini']);
                $arrfim = explode("-", $arr['mesanofim']);

                $valorIni = $arrini[0] ? $arrini : $sifvigenciadtini;
                $valorFim = $arrfim[0] ? $arrfim : $sifvigenciadtfim;

                // Cursista
                $arriniCur = explode("-", $arrCur['mesanoini']);
                $arrfimCur = explode("-", $arrCur['mesanofim']);

                $valorIniCur = $arriniCur[0] ? $arriniCur : $sifvigenciadtini;
                $valorFimCur = $arrfimCur[0] ? $arrfimCur : $sifvigenciadtfim;
                
                $hab 	= 'S';
                $habCur = 'S';
                
                if ($arrini[0] || $arrfim[0]) $colorline = '#FF6A6A';
                else $colorline = '#BFEFFF';
                
                
                ?>

                <?php if ($uc['curdesc'] != $auxCurso) { ?> 
                    <tr>
                        <td align="left" colspan="7"  class="SubTituloDireita" style="text-align: left;font-size:x-small;"> <?php echo $uc['curdesc'] ?></td>
                    </tr>
                    <tr>

                        <td class="SubTituloCentro" width="10%" style="font-size:x-small;">Unidade</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Início da vigência</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Fim da Vigência</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Período de referência(Início)</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Período de referência(Término)</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Início Cursista</td>
                        <td class="SubTituloCentro" style="font-size:x-small;">Término Cursista</td>
                    </tr>
            <?php
        }
        $auxCurso = $uc['curdesc'];
        ?>


                <tr style="background-color: <?=$colorline ?>">
                    <td align="left" style="font-size:x-small;"><?php if ('S' == $hab) { ?><input style="margin-right: 5px;" type="checkbox" name="curso[]" value="<?php echo $uc['sifid']; ?>" class="check_cursos"/><?php } ?>
                        <?= $uc['uninome'] ?></td>
                    <td align="center" style="font-size:x-small;"><?= $sifvigenciadtini ? $sifvigenciadtini[2] . '/' . $sifvigenciadtini[1] . '/' . $sifvigenciadtini[0] : '-'; ?></td>
                    <td align="center" style="font-size:x-small;"><?= $sifvigenciadtfim ? $sifvigenciadtfim[2] . '/' . $sifvigenciadtfim[1] . '/' . $sifvigenciadtfim[0] : '-'; ?></td>
                    <td align="center"><? $db->monta_combo($hab . 'mesini[' . $uc['sifid'] . ']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesini_' . $uc['sifid'], '', $valorIni[1], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?> / <? $db->monta_combo($hab . 'anoinicio[' . $uc['sifid'] . ']', $sql_ano, $hab, 'Selecione', '', '', '', '80', 'N', 'anoinicio', '', $valorIni[0], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?></td>
                    <td align="center"><? $db->monta_combo($hab . 'mesfim[' . $uc['sifid'] . ']', $sql_mes, $hab, 'Selecione', '', '', '', '', 'N', 'mesfim_' . $uc['sifid'], '', $valorFim[1], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?> / <? $db->monta_combo($hab . 'anofim[' . $uc['sifid'] . ']', $sql_ano, $hab, 'Selecione', '', '', '', '80', 'N', 'anofim', '', $valorFim[0], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?></td>
                    <td align="center"><? $db->monta_combo($habCur . 'mesinicur[' . $uc['sifid'] . ']', $sql_mes, $habCur, 'Selecione', '', '', '', '', 'N', 'mesinicur_' . $uc['sifid'], '', $valorIniCur[1], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?> / <? $db->monta_combo($habCur . 'anoiniciocur[' . $uc['sifid'] . ']', $sql_ano, $habCur, 'Selecione', '', '', '', '80', 'N', 'anoiniciocur', '', $valorIniCur[0], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?></td>
                    <td align="center"><? $db->monta_combo($habCur . 'mesfimcur[' . $uc['sifid'] . ']', $sql_mes, $habCur, 'Selecione', '', '', '', '', 'N', 'mesfimcur_' . $uc['sifid'], '', $valorFimCur[1], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?> / <? $db->monta_combo($habCur . 'anofimcur[' . $uc['sifid'] . ']', $sql_ano, $habCur, 'Selecione', '', '', '', '80', 'N', 'anofimcur', '', $valorFimCur[0], null, 'onchange="selecionarLinhaPeriodoExecucao(this);" style="font-size:x-small;"'); ?></td>
                </tr>
    <? endforeach; ?>
<? endif; ?>
        <tr>
            <td class="SubTituloDireita" width="20%">&nbsp;</td>
            <td>
                <input type="submit" id="salvaconfiguracoes" value="Salvar">
                <input type="button" value="Próximo" onclick="divCarregando();
                        window.location = 'sisfor.php?modulo=principal/mec/mec&acao=A&aba=gerenciarusuario';">
            </td>
        </tr>
    </table>
</form>

<? endif; ?>
