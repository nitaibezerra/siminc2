<?
/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: consultalimites.inc
		Finalidade: relatório para os valores limites da proposta agrupado por Grupo de matrizes, Matrizes e Recursos (Fontes - Tesouro e Próprios)

$grupo = codigo do grupo matriz valor default = 0
	--> O valor 0 indica que é o total geral e se for diferente de 0 é o somatório por grupo
$ppoid = código da proposta
$unicod = código da unidade
Função para retornar o somatório da proposta por grupo matriz e/ou com todos os grupos matrizes, por unidade
*/

$ppoid = $db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '".$_SESSION['exercicio']."'");
if (!$ppoid){
?>
    <script>
        alert('Não foi encontrado a proposta ativa para o Exercício de <?=$_SESSION['exercicio']?> !');
	history.back(-1);
    </script>
<?
    exit;
}


if ($_REQUEST['relat']=='planilha_lista'){
    unset ($_REQUEST['relat']);
    if ($_REQUEST['planilha']=='4'){
        header('Content-type: application/xls');
        header('Content-Disposition: attachment; filename="planilha_simec.xls"');
        $proximaLinha = "\n";
        $proximaColuna = "\t";
    }
    else if ($_REQUEST['planilha']=='6'){
        header('Content-type: application/csv');
        header('Content-Disposition: attachment; filename="planilha_simec.csv"');	
        $proximaLinha = "\n";
        $proximaColuna = ",";
    }
    for ($i=0;$i<count($_SESSION['Query']);$i++){
        $RS = $db->record_set($_SESSION['Query'][$i]);
  	$_SESSION['conteudo']=  $db->carrega_tudo($RS); 
	$_SESSION['LINHA'] = $db->conta_linhas($RS);
	$_SESSION['title'] = $tit;
	$_SESSION['TITULO']= 'Relatório Gerencial de Ação';
	$_SESSION['TIT'][0][0] = 'Código ';
	$_SESSION['TIT'][0][1] = 'Título';
	$_SESSION['data']= date('(h:m) d/m/Y');
	$_SESSION['LIN'] = array();

	$_SESSION['cab'] = Array();
	for ($a=0;$a<pg_num_fields($RS);$a++){
            $_SESSION['cab'][$a] = pg_field_name($RS,$a); 
	}
	
	for ($a=0;$a<count($_SESSION['cab']);$a++){
            if ($a<count($_SESSION['cab'])-1){ 
                print trim(($_SESSION['cab'][$a])). ' ' . $proximaColuna;
            }else{
                print($_SESSION['cab'][$a]);
            }
        }
    	
	print $proximaLinha;

        if ($_SESSION['LINHA'] >= 0){
            for ($a=0; $a<=$_SESSION['LINHA'];$a++){
                $res = $db->carrega_registro($RS,$a);
                // a linha abaixo transforma em variáveis todos os campos do array
                if (is_array($res)) {
                    foreach($res as $k=>$v) {
                        ${$k} = $v;
                    }
                }
     	    }
        }

        if (count($_SESSION['conteudo'][0])>1){
            for ($a=0;$a<count($_SESSION['conteudo']);$a++){
	        foreach($_SESSION['conteudo'][$a] as $k=>$v){
                    if ($k<>'acao'){
                        ${$k}=$v;
                        if ($v == 't') $v='1';
                        else if ($v=='f') $v='0';
                        if ($v <>''){
                            $v=$v;
                            print $v . $proximaColuna;
                        }
                    }
                }
		print $proximaLinha;
	     }	
        }    
    }
    exit();    
}


function ExecutaQuery ( $grupo , $ppoid , $unicod)
{
	global $db;
	
	$Query ="select ";
			if ($grupo!=0)
	$Query .= " codGrupo, descGrupo, ";

	$Query .= "		SUM ( coalesce ( valorTesouro, 0) ) as valorTesouro, SUM ( coalesce ( valorOutros, 0) ) as valorOutros ".
			"	from ".
			"	( ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, ".
			"			SUM ( coalesce ( lf.lmfvalor, 0) ) as valorTesouro, ".
			"			0 as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte_calc2 lf ON ".
			"		lf.mtmid = mm.mtmid and lf.unicod = '".$unicod."' and (lf.foncod like'1%' or lf.foncod like'3%') ".
			"	where mm.ppoid = ".$ppoid." and lf.ppoid = " . $ppoid .
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc ".
			"	union all ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, ".
			"			0 as valorOutros, ".
			"			SUM ( coalesce ( lf.lmfvalor, 0) ) as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte_calc2 lf ON ".
			"		lf.mtmid = mm.mtmid and lf.unicod = '".$unicod."' and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') ".
			"	where mm.ppoid = ".$ppoid." and lf.ppoid = " . $ppoid .
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc ".
			"	) a ";
			if ($grupo!=0)
			{
				$Query .= "	where a.codGrupo = ".$grupo." ".
							"	group by ".
				      		"	codGrupo, descGrupo ";
			}

		$RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
	
			for ($i=0; $i<=$nlinhas;$i++)
			{
			  	$res = $db->carrega_registro($RS,$i);
			}
	

		}
			
			return $res;
}



 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com)
   Módulo:conMontante.inc
   Finalidade: permitir a consulta do cadastro da proposta por Montante Categoria Econômica e Montante por Coluna
 */

if(!isset($_POST['requisicao'])){
   $unidade = $_REQUEST['unicod'];  
   $ppoid = $db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '" . $_SESSION['exercicio'] . "'");

   include APPRAIZ."includes/cabecalho.inc"; 
   if ( !$unidade and $_SESSION["unicod_visualizacao"] )
   {
           $unidade = $_SESSION["unicod_visualizacao"];
   }
   else if ( !$unidade and !$_SESSION["unicod_visualizacao"] )
   {
       exit(
           "<script type=\"text/javascript\">
               alert( 'Selecione a Unidade para Visualizar os Limites!' );
           </script>
           <script type=\"text/javascript\">
               document.location.href = \"elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A\";
           </script>"
       );
   }
}else{
    $unidade = $_POST['dados']['unicod'];
    $ppoid = $_POST['dados']['ppoid'];
    $exercicio = $_POST['dados']['exercicio'];    
}
    if(!isset($_POST['requisicao'])){

?>
<script type="text/javascript">

    function Consultar(codigo) {
	    document.formulario.act.value = "consultar";
	    document.formulario.unidade.value = codigo;
	    document.formulario.submit();
    }
    function SubmeterConsulta(){
        document.formulario.act.value = "listar";
	    document.formulario.unidade.value = '';
	    document.formulario.submit();
    }
    function mudaProposta(){
        document.formulario.act.value = "";
	    document.formulario.unidade.value = '';
	    document.formulario.submit();
    }

</script>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<br>
<?
    
        $db->cria_aba($abacod_tela,$url,$parametros);
	$sql="Select count(ppostatus) as codigo from elabrev.propostaorcamento where ppoanoexercicio = '".$_SESSION['exercicio']."'";	
	$qtdPropostas =(integer)$db->pegaUm( $sql );
    }else{
        $classesAdicionais = ' table-striped table-bordered table-hover';
    }
?>


<form method="POST" name="formulario" style="margin:0px">
    <input type=hidden name="modulo" value="<?=$modulo?>">
    <input type=hidden name="act" value="">
    <input type=hidden name="unidade" value="">

    <input type='hidden' name="relat" >
    <input type='hidden' name="planilha" >


<?
    if(!isset($_POST['requisicao']))
        monta_titulo($titulo_modulo,'');    
?>

<?
    if(!isset($_POST['requisicao'])){    
?>
        <table width="100%" style="width: 100%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="0"  cellPadding="3" border="0" align="center">						
            <tr>
                <td align="center" valign="middle">
                    <b>LIMITES ORÇAMENTÁRIOS</b>
                </td>
            </tr>
            <tr>
                <td align="center" valign="middle">
                    <b>
                    <?	$NomeProposta=$db->pegaUm( "select ppodsc || ' - ' || ppoano as descricao from elabrev.propostaorcamento where ppoid = ".(integer)$ppoid.""); 
                    print str_to_upper($NomeProposta);
                    ?>
                    </b>
                </td>
            </tr>
            <tr>
                <td align="center" valign="middle">
                    <b>ATIVIDADES - PROJETOS E OPERAÇÕES ESPECIAIS</b>
                </td>
            </tr>
            <tr>
                <td align="center" valign="middle"><b>
                    <font color="black" size="2">
                    <?
                    $NomeUnidade=$db->pegaUm( "select unidsc as descricao from unidade where unicod = '".$unidade."'"); 
                    print str_to_upper($unidade.' - '.$NomeUnidade);
                    ?></font></b>
                </td>
            </tr>
        </table>
<?
    }    
?>

<table class="tabela <?php echo $classesAdicionais; ?>" style="width: 100%;" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1"  align="center">
    <tr bgcolor="#DCDCDC">
        <td align="center" width="55%" rowspan="2" valign="middle"><b>DISCRIMINAÇÃO</b></td>
	<td colspan="3" width="45%" align="center"><b>LIMITES</b></td>
    </tr>
    <tr bgcolor="#DCDCDC">
        <td align="center" width="15%"><b>TESOURO<br>(a)</b></td>
	<td align="center" width="15%"><b>PRÓPRIOS<br>(b)</b></td>
	<td align="center" width="15%"><b>TOTAL<br>(c=a+b)</b></td>
    </tr>
<?  
    $Query1 ="
        SELECT 
            codGrupo, 
            descGrupo, 
            codigo, 
            descricao, 
            SUM ( coalesce ( valorTesouro, 0) ) as valorTesouro, 
            SUM ( coalesce ( valorOutros, 0) ) as valorOutros 
        FROM (
            SELECT gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, gm.gpmordem, 
                SUM ( coalesce ( lf.lmfvalor, 0) ) as valorTesouro,
		0 as valorOutros 
            FROM elabrev.grupomatriz gm
            INNER JOIN elabrev.matriz m ON m.gpmid = gm.gpmid 
            INNER JOIN elabrev.montantematriz mm ON mm.mtrid = m.mtrid 
            LEFT JOIN elabrev.limitefonte_calc2 lf ON lf.mtmid = mm.mtmid and lf.unicod = '".$unidade."' and (lf.foncod like'1%' or lf.foncod like'3%') 
            WHERE mm.ppoid = ".$ppoid." and lf.ppoid = $ppoid 
            GROUP BY gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, gm.gpmordem 
            UNION ALL
            SELECT gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, gm.gpmordem, 
                0 as valorTesouro, 			
		SUM ( coalesce ( lf.lmfvalor, 0) ) as valorOutros 
            FROM elabrev.grupomatriz gm 
            INNER JOIN elabrev.matriz m ON m.gpmid = gm.gpmid 
            INNER JOIN elabrev.montantematriz mm ON mm.mtrid = m.mtrid 
            LEFT JOIN elabrev.limitefonte_calc2 lf ON lf.mtmid = mm.mtmid and lf.unicod = '".$unidade."' and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') 
            WHERE mm.ppoid = $ppoid  and lf.ppoid = $ppoid
            GROUP BY gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, gm.gpmordem
            ) a 
	GROUP BY
            codGrupo, descGrupo, codigo, descricao, gpmordem having ( SUM ( coalesce ( valorTesouro, 0) ) > 0 or SUM ( coalesce ( valorOutros, 0) ) > 0 )
	ORDER BY
            gpmordem, descGrupo, descricao 
    ";

    $RS = $db->record_set($Query1);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {	
        $grupo = 0;
	$grupoNovo = 0;
	for ($i=0; $i<=$nlinhas;$i++)
	{
            $res = $db->carrega_registro($RS,$i);
            $grupoNovo = $res[0];
            if ($i == 0)
            {
                $grupo = $res[0];
		$grupoNovo = $res[0];
		$a = ExecutaQuery ($res[0],$ppoid,$unidade);
?>
                <tr bgcolor="#DCDCDC">
                    <td >
                        <b>
                        <?=$res[1]?></b></td><td align="right"><b><?=number_format($a[2], 0, ',', '.')?></b></td><td align="right"><b><?=number_format($a[3], 0, ',', '.')?></b></td><td align="right"><b><?=number_format( ( $a[2]+$a[3] ) , 0, ',', '.')?>
                        </b>
                    </td>
                </tr>	
                <tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
<?          }

            if ($grupoNovo == $grupo)
            {
                $grupo = $grupoNovo;
            }else {
                $b = ExecutaQuery ($res[0],$ppoid,$unidade);
?>
                <tr>
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
                </tr>
                <tr bgcolor="#DCDCDC">
                    <td>
                        <b><?=$res[1]?></b>
                    </td>
                    <td align="right">
                        <b><?=number_format($b[2], 0, ',', '.')?>
                        </b>
                    </td>
                    <td align="right"><b><?=number_format($b[3], 0, ',', '.')?></b></td>
                    <td align="right"><b><?=number_format( ( $b[2]+$b[3] ) , 0, ',', '.')?></b></td>
                </tr>	
                <tr>
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
                    <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
                </tr>
<?		  	

                $grupo = $res[0];
            }
?>
        <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
            <td class="SemBordaCimaBaixo">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$res[3]?></td>
            <td class="SemBordaCimaBaixo" align="right"><?=number_format($res[4], 0, ',', '.')?></td>
            <td class="SemBordaCimaBaixo" align="right"><?=number_format($res[5], 0, ',', '.')?></td>
            <td class="SemBordaCimaBaixo" align="right"><?=number_format( ( $res[4]+$res[5] ) , 0, ',', '.')?></td> 
        </tr>						

<?
        }
        $c = ExecutaQuery (0,$ppoid,$unidade);
?>
	<tr>
            <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
            <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> 
            <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
        </tr>
	<tr bgcolor="#DCDCDC">
            <td align="center"><b>TOTAL</b></td>
            <td align="right"><b><?=number_format($c[0], 0, ',', '.')?></b></td>
            <td align="right"><b><?=number_format($c[1], 0, ',', '.')?></b></td>
            <td align="right"><b><?=number_format( ($c[0]+$c[1]) , 0, ',', '.')?></b></td>
        </tr>	

<?
    }
?>
</table>

<br>
<table style="width: 100%;" class="tabela <?php echo $classesAdicionais; ?>" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1" align="center">	
    <tr bgcolor="#DCDCDC">
        <td colspan="2" width="55%" align="center"><b>FONTES</b></td>
	<td align="center" width="15%"><b>LIMITE</b></td>
	<td align="center" width="15%"><b>DESPESAS PROGRAMADAS</b></td>
	<td align="center" width="15%"><b>SALDO A PROGRAMAR</b></td>
    </tr>		
<?
    $Query2 = "
        SELECT
            codigo, 
            descricao, 
            SUM ( coalesce ( vlLimite, 0) ) as vlLimite, 
            SUM ( coalesce ( vlDespesa, 0) ) as vlDespesa, 
            SUM ( coalesce ( vlLimite, 0) - coalesce ( vlDespesa, 0) ) as vlSaldo 
	FROM
            ( SELECT 	 
                fr.foncod as Codigo, 
		fr.fondsc as Descricao, 
		SUM ( coalesce ( lf.lmfvalor, 0) ) as vlLimite, 
		0 as vlDespesa 
            FROM elabrev.limitefonte_calc2 lf 
            INNER JOIN unidade u ON u.unicod = lf.unicod and u.unitpocod = lf.unitpocod 
            INNER JOIN elabrev.montantematriz mm ON mm.mtmid = lf.mtmid 
            INNER JOIN elabrev.matriz m ON m.mtrid = mm.mtrid 
            INNER JOIN elabrev.propostaorcamento po ON po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "'
            INNER JOIN public.fonterecurso fr ON fr.foncod = lf.foncod 
            WHERE po.ppoid = ".$ppoid." and lf.ppoid = ".$ppoid." and u.unicod = '".$unidade."' 
            GROUP BY fr.foncod, fr.fondsc 
            UNION ALL 
            SELECT
		fr2.foncod, 
		fr2.fondsc, 
		0 as vlLimite, 
		SUM(coalesce (da.dpavalor + da.dpavalorexpansao,0)) as vlDespesa 
            FROM elabrev.despesaacao da 
            INNER JOIN elabrev.ppaacao_orcamento ac ON ac.acaid = da.acaid 
            INNER JOIN public.fonterecurso fr2 ON fr2.foncod = da.foncod 
            WHERE ac.unicod = '".$unidade."' and da.ppoid =  $ppoid 
            GROUP BY fr2.foncod, fr2.fondsc 
            ) as foo 
        GROUP BY
            codigo, 
            descricao 
        ORDER BY codigo 
    ";

    $RS = $db->record_set($Query2);
    $nlinhas = $db->conta_linhas($RS);
    if ($nlinhas>=0)
    {
?>
        <tr>
            <td align="center" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="left" width="40%" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
            <td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
	</tr>

<?
        $grupo = 0;
	$grupoNovo = 0;
	$vlTotal = 0;
	$vlDespesa = 0;
		
	for ($i=0; $i<=$nlinhas;$i++)
        {
            $res = $db->carrega_registro($RS,$i);
            $vlTotal = $vlTotal + $res[2];
            $vlDespesa = $vlDespesa + $res[3];		  	
?>
            <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
                <td align="center" class="SemBordaCimaBaixo"><?=$res[0]?></td>
		<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[1] )?></td>
		<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[2], 0, ',', '.')?></td>
		<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[3], 0, ',', '.')?></td>
		<td align="right" class="SemBordaCimaBaixo"><?=number_format(($res[2]-$res[3]), 0, ',', '.')?></td>
            </tr>
<?
        }
?>
	<tr>
            <td align="center" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
            <td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
            <td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
	</tr>
	<tr bgcolor="#DCDCDC">
            <td colspan="2" align="center"><b>TOTAL</b></td>
            <td align="right"><b><?=number_format($vlTotal, 0, ',', '.')?></b></td>
            <td align="right"><b><?=number_format($vlDespesa, 0, ',', '.')?></b></td>
            <td align="right"><b><?=number_format(($vlTotal-$vlDespesa), 0, ',', '.')?></b></td>
	</tr>
<?
    }
    if(!isset($_POST['requisicao'])){
?>
        <tr bgcolor="#DCDCDC">
            <td colspan="5" align="center">
                <INPUT TYPE="button" NAME="Voltar" VALUE="Voltar" class="notprint" onclick="history.back();">
            </td>
        </tr>
<? 
    }
?>
</table>
<?	
    $_SESSION['Query'] = Array($Query1,$Query2);
    if(!isset($_POST['requisicao'])){
        
        print '<table class="tabela '. $classesAdicionais.'" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
        if ($_SESSION['LINHA'] >=0) 
            print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(4)">';
            print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(6)">';      
            print '</td></tr></table>';

    }
?>
</form>
<?php
    if(!isset($_POST['requisicao'])){    
?>
        <script>
            function imprime_rel(cod){
                <?
                $browser = get_browser();
                $saiu = false;
                if (is_array($browser)){
                    foreach ($browser as $name => $value) {
                        if ($value == 'IE') {
                            $saiu = true;
                        }
                    }

                }
                if ($saiu){
                ?>
                    if (cod == 1) {
                        location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
                    };
                    if (cod== 2) {
                        location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
                    }
                    if (cod>2) {
                        if (cod ==3) document.formulario.relat.value='planilha';
                        if (cod ==4) document.formulario.relat.value='planilha_lista'; 
                        if (cod ==5) document.formulario.relat.value='planilha';
                        if (cod ==6) document.formulario.relat.value='planilha_lista';             	
                        document.formulario.planilha.value=cod;         	
                        document.formulario.submit();
                    }
                <?

                } else { 
                ?>
                    if (cod == 1) {
                        e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
                        window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
                    }
                    if (cod== 2) {
                        e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
                        window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
                    }
                    if (cod>2) {
                        if (cod ==3) document.formulario.relat.value='planilha';
                        if (cod ==4) document.formulario.relat.value='planilha_lista'; 
                        if (cod ==5) document.formulario.relat.value='planilha';
                        if (cod ==6) document.formulario.relat.value='planilha_lista';          	
                        document.formulario.planilha.value=cod;         	
                        document.formulario.submit();
                    }  


                <? 

                } 
                ?>
            }
        </script>
<?
    }
?>