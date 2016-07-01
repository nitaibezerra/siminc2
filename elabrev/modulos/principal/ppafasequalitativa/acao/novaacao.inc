<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:novaacao.inc
   Finalidade: permitir a inclusão de uma nova acao
   */
$modulo=$_REQUEST['modulo'] ;//
			$acaid = (integer) $_REQUEST['acaid'];

function atualiza_unidade($acaid, $unicod )
	{
		global $db;
		$acaid = (integer) $acaid;
		if ( !$acaid )
		{
			return;
		}
		$sql_remocao = "delete from elabrev.unidade_acao where acaid = " . $acaid;
		$db->executar( $sql_remocao );
		$sql_insercao = "insert into elabrev.unidade_acao (acaid,unicod,usucpf ) values ( " . $acaid . ", '" . $unicod . "','".$_SESSION['usucpf']."' )";
		$db->executar( $sql_insercao );
	}	
	
/*
function my_ucwords($s)
       {
       $a=strtolower($s);
       $s=ucfirst($a);
       for($x=0; $x<strlen($s)-1; $x++)
           if(!ctype_alpha($s[$x])) $s[$x+1]=strtoupper($s[$x+1]);

       //mantem minuscula artigo, preposição, conjunção, etc
       $minuscole=array("em", "o", "a", "um", "uns", "uma", 
                 "umas", "no", "para",                      
                 "até",  "de", "à", "e", "numa",            
                 "então", "a", "da", "do", "com", "mas", "porem");
      
       foreach($minuscole as $value)
           {
           $pos=strpos($a, $value);
           if( ( $pos>0 && $pos<strlen($s)-1 && !ctype_alpha($a[$pos-1]) && !ctype_alpha($a[$pos+1]) )    //CARATTERE IN MEZZO
           ||  ( $pos==strlen($s)-1 && !ctype_alpha($a[$pos-1]) ) )   //CASO PARTICOLARE: carattere in fondo. x es: "hotel 2000's"
               $s[$pos]=strtolower($s[$pos]);
           }
       return $s;
       }
*/

/**
 * Transforma a primeira letra de cada letra em maiúsculas, exceto as
 * pertencentes a lista indicada na variável $minuscole.
 *
 * @param string $string
 * @return string
 */
function my_ucwords( $string )
{
	$string = strtolower( $string );	
	$minuscole = array(
		'a', 'à', 'e', 'é', 'o',
		'da', 'de', 'do',
		'com', 'para',
		'mas', 'porém', 'então',
		'até',
		'lá',
		'em', 'no', 'na', 'num', 'nuns', 'numa', 'numas',
		'um', 'uns', 'uma', 'umas'
	);
	$result = '';
	foreach ( split( '[[:space:]]', $string ) as $word )
	{
		if ( in_array( $word, $minuscole ) )
		{
			$result .= ' ' . strtolower( $word );
		}
		else
		{
			$result .= ' ' . ucfirst( $word );
		}
	}
	return ucfirst( substr( $result, 1 ) );
}

if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
	foreach($_REQUEST as $k=>$v) ${$k}=$v;
	// mandou gravar
	$acadsc=my_ucwords($acadsc);
	if ($acaoorcamento=='t') {$acasnfiscalseguridade='t';$acasninvestatais='f';} else
	{$acasnfiscalseguridade='f';$acasninvestatais='t';}; 
	if (! $acavlrcustototal) $acavlrcustototal=0;
	if (! $acaqtdcustototal) $acaqtdcustototal=0;
	if (! $acavlrrepercfinanceira) $acavlrrepercfinanceira=0;
	if ($acasnmedireta<>'t') $acasnmedireta='f';
	if ($acasnmedesc<>'t') $acasnmedesc='f';
	if (! $ungcod) $ungcod=null;	
	if ($acasnmelincred<>'t') $acasnmelincred='f';	
	if ($acasntransfobrigatoria<>'t') $acasntransfobrigatoria='f';	
	if ($acasntransfvoluntaria<>'t') $acasntransfvoluntaria='f';
	if ($acasntransfoutras<>'t') $acasntransfoutras='f';
	if ($acasndespesaobrigatoria<>'t') $acasndespesaobrigatoria='f';
	if ($acasnbloqueioprogramacao<>'t') $acasnbloqueioprogramacao='f';
	if ($acasnpadronizada<>'t') $acasnpadronizada='f';
	if ($acasnsetpadronizada<>'t') $acasnsetpadronizada='f';
	if ($acasnfiscalseguridade<>'t') $acasnfiscalseguridade='f';
	if ($acasninvestatais<>'t') $acasninvestatais='f';
	
	// força a gravação do campo transferência
	$acasntransfobrigatoria = 'f';
	$acasntransfvoluntaria  = 'f';
	$acasntransfoutras      = 'f';
	if ( $acatransfer == 't' ) {
		switch ( $formaimplementacao ) {
			case 'to':
				$acasntransfobrigatoria = 't';
				break;
			case 'tv':
				$acasntransfvoluntaria = 't';
				break;
			case 'ou':
				$acasntransfoutras = 't';
				break;
		}
	}

    $sql1 = "select prgcod from ppaprograma_proposta where prgid=".$prgid;
	$prgcod = $db->pegaUm($sql1);
	
    if ($_REQUEST['acao_n']=='altera') 
    { 
    	$sql =
    		"update elabrev.ppaacao_proposta set
				prgcod='$prgcod',
				prgid='$prgid',
				funcod='$funcod',
				sfucod='$sfucod',
				taccod='$taccod',
				procod =$procod,
				acadscproduto='$acadscproduto',
				unmcod=$unmcod,
				esfcod=$esfcod,
				acadsc='$acadsc',
				acafinalidade='$acafinalidade',
				acadescricao='$acadescricao',
				acabaselegal ='$acabaselegal',
				acarepercfinanceira='$acarepercfinanceira',
				acavlrrepercfinanceira=$acavlrrepercfinanceira,
				acasnmedireta='$acasnmedireta',
				acasnmedesc='$acasnmedesc',
				acasnmelincred ='$acasnmelincred',
				acasndespesaobrigatoria ='$acasndespesaobrigatoria',
				acasnbloqueioprogramacao='f',
				acadetalhamento='$acadetalhamento',
				acamesinicio='$acamesinicio',
				acaanoinicio='$acaanoinicio',
				acamestermino ='$acamestermino',
				acaanotermino='$acaanotermino',
				acavlrcustototal=$acavlrcustototal,
				acaqtdcustototal=$acaqtdcustototal,
				acaoobs='$acaoobs',
				ungcod='$ungcod',
				unicod='$unicod',
				acasnmetanaocumulativa='$acasnmetanaocumulativa',
				acasnoutrasfontes='$acasnoutrasfontes',
				acasnpadronizada='$acasnpadronizada',
				acasnsetpadronizada='$acasnsetpadronizada',
				acasninvestatais='$acasninvestatais',
				acasnfiscalseguridade='$acasnfiscalseguridade',

				acasntransfobrigatoria='$acasntransfobrigatoria',
				acasntransfvoluntaria='$acasntransfvoluntaria',
				acasntransfoutras='$acasntransfoutras'

			where acaid=$acaid ";
    } else {
        $acacod='XXXX';
    	$sql =
    		"insert into elabrev.ppaacao_proposta (
				prgcod,
				prgid,
				funcod,
				sfucod,
				taccod,
				procod,
				acadscproduto,
				unmcod,
				esfcod,
				acadsc,
				acafinalidade,
				acadescricao,
				acabaselegal,
				acarepercfinanceira,
				acavlrrepercfinanceira,
				acasnmedireta,
				acasnmedesc,
				acasnmelincred,
				acasndespesaobrigatoria,
				acasnbloqueioprogramacao,
				acadetalhamento,
				acamesinicio,
				acaanoinicio,
				acamestermino ,
				acaanotermino,
				acavlrcustototal,
				acaqtdcustototal,
				acaoobs,
				acastatus,
				prsano,
				acacod,
				usucpf,
				ungcod,
				unicod,
				acasnmetanaocumulativa,
				acasnoutrasfontes,
				acasnpadronizada,
				acasnsetpadronizada,
				acasntransfobrigatoria,
				acasntransfvoluntaria,
				acasntransfoutras
			) values (
				'$prgcod',
				$prgid,
				'$funcod',
				'$sfucod',
				'$taccod',
				$procod,
				'$acadscproduto',
				$unmcod,
				$esfcod,
				'$acadsc',
				'$acafinalidade',
				'$acadescricao',
				'$acabaselegal',
				'$acarepercfinanceira',
				$acavlrrepercfinanceira,
				'$acasnmedireta',
				'$acasnmedesc',
				'$acasnmelincred',
				'$acasndespesaobrigatoria',
				'f',
				'$acadetalhamento',
				'$acamesinicio',
				'$acaanoinicio',
				'$acamestermino',
				'$acaanotermino',
				$acavlrcustototal,
				$acaqtdcustototal,
				'$acaoobs',
				'N',
				'".$_SESSION['exercicio']."',
				'$acacod',
				'".$_SESSION['usucpf']."',
				'$ungcod',
				'$unicod',
				'$acasnmetanaocumulativa',
				'$acasnoutrasfontes',
				'$acasnpadronizada',
				'$acasnsetpadronizada',
				'$acasntransfobrigatoria',
				'$acasntransfvoluntaria',
				'$acasntransfoutras'
			)";
    }
//    dbg( $sql, 1 );
    $res = $db->executar($sql);
    atualiza_unidade($acaid, $unicod);
    $db->commit();
    $db->sucesso($modulo);    	
}


if ($_REQUEST['acao_n']=='exclui')
{
    $sql = "delete from elabrev.ppaacao_proposta where acaid=".$_REQUEST['acaid'];
    $res = $db->executar($sql);

    $db->commit();
    $db->sucesso($modulo);

}
foreach($_REQUEST as $k=>$v) ${$k}=$v;
if ($_REQUEST['acaid'] or $_REQUEST['veracaid'])
{
	if ($_REQUEST['veracaid']) $_REQUEST['acaid']=$_REQUEST['veracaid'];
     $sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.prgid=b.prgid and a.acaid=".$_REQUEST['acaid'];
    $res = $db->recuperar($sql);

    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    if ($acasnfiscalseguridade=='t') $acaoorcamento='t'; else $acaoorcamento='f';
    $_SESSION['acaid']=$_REQUEST['acaid'];
}
//
if ($_REQUEST['clone'])
{
     $sql = "select a.*, b.prgdsc from ".$_SESSION['sisdiretorio'].".ppaacao_proposta a, ".$_SESSION['sisdiretorio'].".ppaprograma_proposta b where a.prgid=b.prgid and a.acaid=".$_REQUEST['clone'];
$saida = $db->recuperar($sql,$res);
   if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}
       if ($acasnfiscalseguridade=='t') $acaoorcamento='t'; else $acaoorcamento='f';
   $msg='Dados obtidos a partir de clonagem!';
}
if ($_REQUEST['acasnoutrasfontes']) $acasnoutrasfontes=$_REQUEST['acasnoutrasfontes'];
if ($_REQUEST['taccod']) $taccod=$_REQUEST['taccod'];
 

include APPRAIZ."includes/cabecalho.inc";

$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo='Inclusão de uma nova Ação';
monta_titulo($titulo_modulo,'');
// variáveis gerais
$sql_locgasto = "select distinct p.regcod as codigo, p.regcod || ' - ' || r.regdsc as descricao from ppalocalizador p inner join regiao r on r.regcod=p.regcod left join elabrev.localizadorgasto_acao lga on lga.regcod=p.regcod and lga.acaid=".$_REQUEST['acaid']." order by descricao";

?>

<script language="Javascript" src="/includes/livesearch.js"></script>
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="clone" value=0>
<input type=hidden name="registro" value=<?=$_POST['registro'];?>>
<input type=hidden name="acao_n">
<input type=hidden name="veracaid" value=0>
<input type=hidden name="acaid" value=<?=$_REQUEST['acaid']?>>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

      <?if ($_REQUEST['acaid']) {
      	      	if (! $_REQUEST['veracaid']) {
      	?>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td>
        <input type="button" name="btnAbrirResp" TITLE="Se esta ação está recebendo dados de outra (Fusão), clique aqui para indicar." value="Fusão" onclick="popfun(<?=$_REQUEST['acaid']?>)"></td>
     </tr> 
      <?}?>
      <!--<tr>
        <td align='right' class="SubTituloDireita">Unidades Orçamentárias relacionadas</td>
        <td><?
             $sql = "select u.unicod as codigo, u.unicod||'-'||u.unidsc as descricao from unidade u inner join elabrev.unidade_acao ua on ua.unicod=u.unicod and ua.acaid = ".$_REQUEST['acaid']."  where u.unitpocod='U' order by u.unicod ";
            $db->monta_combo('unicod',$sql,'S','Lista das Unidades relacionadas a esta nova ação','','','',400);
            print "&nbsp;&nbsp;<img src='../imagens/alterar.gif' align='absmiddle' width='15' height='15' title='Incluir Unidades Orçamentárias' onclick=".'"incluir_unid('.$_REQUEST['acaid'].')">'?></td>      
      </tr>-->
       
      <?} else   if ($db->verifica_momento())
   {?>
      <tr>
        <td align='right' class="SubTituloDireita"></td>
        <td><input type="button" name="btngeraclone" value="Gerar Clone." onclick="geraclone()">&nbsp;&nbsp;<?=$msg?></td>
      </tr>       
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão:</td>
        <td >
	<?
   $orgcod =$_SESSION["ittorgao"];
    $sql = "select orgcod as CODIGO,orgdsc as DESCRICAO from orgao where organo='".$_SESSION['exercicio']."' order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'N',"Todos os Órgãos",'atualizaComboUnidade','','MEC',400);
	?></td>
      </tr>
	<?
	if ($_REQUEST['unicod']) $unicod=$_REQUEST['unicod'];
	  if ($orgcod) {
	  	$habil='S';
        ?>
      <tr bgcolor="#F2F2F2">
        <td align='right' class="SubTituloDireita">Unidade Orçamentária (UO):</td>
         <td >
         <?
         if (! $_SESSION['uniorc']) 
		$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' order by unidsc ";
		else 
		{
			$sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' and unicod in (";
			for ($ii=0;$ii<count($_SESSION['uniorc']);$ii++)
			{
				if ($ii > 0 and $ii<count($_SESSION['uniorc'])-1) $sql .= ",";
				$sql .="'".$_SESSION['uniorc'][$ii]."'";
				//if ($ii == 0 and count($_SESSION['uniorc'])>0) $sql .= ",";
				//if ($ii > 0 and $ii<count($_SESSION['uniorc'])-1) $sql .= ",";
			}
			$sql .=") order by unidsc ";
	  	}

			$db->monta_combo("unicod",$sql,'S',"Todas as Unidades",'escolhe_gestora','','Escolha a Unidade Orçamentária',400);	  	
		
?>
	</td>
      </tr>
 
      	  <?
	      $org = substr($_SESSION['ittorgao'],0,2);
	  if (($unicod == $org.'101' or $unicod == $org.'298') and $orgcod==$_SESSION['ittorgao']) {?>
      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora (UG):</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='26101' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Escolha a Unidade Gestora",'','','Escolha a Unidade Gestora',400);	  
	   
	?>
	</td>
      </tr>
      <?}?>

      
<?}?>      
	 <tr align="left">
        <td align='left' class="SubTituloDireita">Programa:</td>
        <td>
        <? $prgid=$prgid;
        $sql = "select p.prgid as codigo, p.prgcod||'-'||prgdsc as descricao from elabrev.ppaprograma_proposta p where p.prgstatus='A' and p.prsano='".$_SESSION['exercicio']."' order by descricao";
            $db->monta_combo('prgid',$sql,'S','Escolha o Programa','','','',400);?>
        </td>
      </tr>

<!--      <tr>
        <td align='right' class="SubTituloDireita">Unidade Orçamentária Responsável:</td>
        <td>
        <? 
        $sql = "select u.unicod as codigo, u.unicod||'-'||u.unidsc as descricao from unidade u inner join orgao o on o.orgcod=u.orgcod  where o.organo='".$_SESSION['exercicio']."' and u.orgcod='".$orgcod."' and unistatus='A' order by descricao";
            $db->monta_combo('unicod',$sql,'S','Escolha a Unidade','','','Escolha a Unidade',400);?>
        </td>         
      </tr>
            <tr>
        <td align='right' class="SubTituloDireita">Código (:</td><?
        $sql = "select distinct a.acacod as valor, a.acacod as descricao FROM elabrev.ppaacao a WHERE a.acastatus='A' AND a.prsano='".$_SESSION['exercicio']."' AND a.acacod LIKE '%s%' ORDER BY descricao";
        ?>
        <td><?=campo_texto_ajax($sql, 'acacod_n_ajax', 'acacod_n', 'Código da nova ação', '', 6, 4, '', '', true)?></td>
      </tr>
      
      
      
-->
        <tr>
	      <td align='right' class="SubTituloDireita">Esfera:</td>
        <td><?
$sql= "select esfcod as codigo, esfdsc as descricao from esfera where esfstatus='A' order by descricao";
$db->monta_combo('esfcod',$sql,'S','Escolha a Esfera','','','',400);
?></td>           
       </tr>      

      <tr>
        <td align='right' class="SubTituloDireita">Função:</td>
        <td><?
$sql= "select funcod as codigo, funcod||'-'||fundsc as descricao from ppafuncao where funstatus='A' order by descricao";
$db->monta_combo('funcod',$sql,'S','Escolha a Função','','','Escolha a função',400);
?>
        </td>
     </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Subfunção:</td>
        <td><?
$sql= "select sfucod as codigo, sfucod||'-'||sfudsc as descricao from elabrev.ppasubfuncao where  sfustatus='A' order by descricao";
$db->monta_combo('sfucod',$sql,'S','Escolha a Subfunção','','','Escolha a subfunção',400);
?></td>      
      </tr>       
        
 <!--     <tr>
        <td align='right' class="SubTituloDireita">Código (Deixe em branco):</td>
        <td><?=campo_texto('acacod','N','S','',6,4,'','','','dica','');?></td>
      </tr>
      -->
      <tr>
        <td align='right' class="SubTituloDireita">Título da ação:</td>
        <td><?=campo_textarea('acadsc','S','S','',80,3,'','','','');?></td>
      </tr>

      <tr>
        <td align='right' class="SubTituloDireita">Finalidade:</td>
        <td><?=campo_textarea('acafinalidade','S','S','',80,3,'','','','');?></td>         
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Descrição:</td>  
        <td><?=campo_textarea('acadescricao','S','S','',80,3,'','','','');?></td>          
      </tr>
       <tr>
        <td align='right' class="SubTituloDireita">Produto (Bem ou Serviço):</td>
        <td><?
             $sql= "select procod as codigo, prodsc as descricao from produto where prostatus='A' order by descricao";
            $db->monta_combo('procod',$sql,'S','Escolha o produto','','','',400);
?>
	</td> 
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Especificação do Produto:</td>
        <td><?=campo_textarea('acadscproduto','N','S','',80,3,'','','','');?></td>          
      </tr>      
		<tr>
        <td align='right' class="SubTituloDireita">Unidade de Medida:</td>
        <td><?
$sql= "select unmcod as codigo, unmdsc as descricao from unidademedida where unmstatus='A' order by descricao";
$db->monta_combo('unmcod',$sql,'S','Escolha a unidade de medida','','','Escolha a unidade de medida',400);
?></td> 
      </tr>  
      <tr>
        <td align='right' class="SubTituloDireita">Cumulatividade da Meta:</td>
        <td><input type="radio" value="f" name="acasnmetanaocumulativa" <?if ($acasnmetanaocumulativa=='f') print '   checked'?>>Cumulativa     <input type="radio" value="t" name="acasnmetanaocumulativa" <?if ($acasnmetanaocumulativa=='t') print '   checked'?>>Não cumulativa</td>
      </tr>          
      <tr>
	     <td align='right' class="SubTituloDireita">Tipo de ação:</td>
        <td><?
        	$sql= "select taccod as codigo, tacdsc as descricao from tipoacao where tacstatus='A' order by descricao";
			$db->monta_combo('taccod',$sql,'S','Escolha o tipo de ação','tipoacao','','Escolha o tipo de ação',400);
?></td> 
      </tr>
 <? if ($taccod==1){?>
	<tr>
	   <td align='right' class="SubTituloDireita">Duração do Projeto<br>(Para ações tipo projeto ou não orçamentárias):</td>
       <td>
          <table border=1>
             <tr>
                <td width='50%'>Início</td>
                <td width='50%'>Término</td>
             </tr>
             <tr>
                <td width='50%'><?=campo_texto('acamesinicio','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('acaanoinicio','N',$habil,'',6,4,'####','N');?></td>
                <td width='50%'><?=campo_texto('acamestermino','N',$habil,'',3,2,'##','N');?>/<?=campo_texto('acaanotermino','N',$habil,'',6,4,'####','N');?></td>
             </tr>
          </table>
       </td>
    </tr> 
    <?}?>                 
     
      <tr>
           <td align='right' class="SubTituloDireita">Quanto ao Orçamento:</td>
           <td><input type="radio" value="f" onclick ="tipoorcamento(this.value)" name="acasnoutrasfontes" <?if ($acasnoutrasfontes=='f') print '   checked'?>>Orçamentária     <input type="radio" value="t" onclick ="tipoorcamento(this.value)" name="acasnoutrasfontes" <?if ($acasnoutrasfontes=='t') print '   checked'?>>Não orçamentária</td>
           
    </tr>
 <? if ($taccod==1 or $acasnoutrasfontes=='t'){?>        
      <tr>
	     <td align='right' class="SubTituloDireita">Custo total estimado<br>(Para ações tipo projeto ou não orçamentárias):</td>
        <td><?=campo_texto('acavlrcustototal','N',$habil,'',12,12,'#########.##','N');?>
        </td> 
      </tr> 
     <?}?>
 <? if ($_REQUEST['acasnoutrasfontes']=='f' or $acasnoutrasfontes=='f'){?>       
      <tr>
           <td align='right' class="SubTituloDireita">Tipo de Orçamento:</td>
           <td>
         <input type="radio" value="t" name="acaoorcamento" <?if ($acaoorcamento=='t') print '   checked'?>>Fiscal/Seguridade Social    <input type="radio" value="f" name="acaoorcamento" <?if ($acaoorcamento=='f') print '   checked'?>>Investimentos Estatais
          </td>
      </tr>
      <tr><td colspan="2"><br></td></tr>
      <tr>
	     <td align='right' class="SubTituloDireita">Forma de Implementação:</td>
         <td>      
		 <table>
           <tr>
             <td >
                Direta<input type="checkbox" value="t" name="acasnmedireta" <?if ($acasnmedireta=='t') print '   checked'?>>
             Descentralizada<input type="checkbox" value="t" name="acasnmedesc" <?if ($acasnmedesc=='t') print '   checked'?>>
             Transferência<input type="checkbox" value="t" name="acatransfer" <?if ($acasntransfobrigatoria=='t' or $acasntransfvoluntaria=='t' or $acasntransfoutras=='t') print '   checked'?>>Linha de Crédito<input type="checkbox" value="t" name="acasnmelincred" <?if ($acasnmelincred=='t') print '   checked'?>>   </td>          
           </tr>
           <tr>
			<td>
                  Transferência Obrigatória<input type="radio" value="to" name="formaimplementacao" <?if ($acasntransfobrigatoria=='t') print '   checked'?>>
             Transferência Voluntária<input type="radio" value="tv" name="formaimplementacao" <?if ($acasntransfvoluntaria=='t') print '   checked'?>>
             Outras<input type="radio" value="ou" name="formaimplementacao" <?if ($acasntransfoutras=='t') print '   checked'?>></td>          
           </tr>           
         </table>          

         </td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Detalhamento da Implementação:</td>
        <td><?=campo_textarea('acadetalhamento','S','S','',80,3,'','','','');?></td>               </tr>          
      <?}?>      

      <tr>
        <td align='right' class="SubTituloDireita">Base legal:</td>
        <td><?=campo_textarea('acabaselegal','S','S','',80,3,'','','','');?></td>          
	</td>
    </tr> 
     <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=campo_textarea('acaoobs','N','S','',80,3,'','','','','');?></td>
      </tr>     
      <tr>
        <td align='right' class="SubTituloDireita">Ações fundidas</td>
        
               
  <?
  if ($acaid)
  {
$sql = "select  pap.prgcod ||'-'||pap.acacod||'.'||pap.acadsc as acao from elabrev.ppaacao_proposta pap inner join elabrev.proposta_fusao_acao pfa on pfa.acaid=$acaid and pfa.acaid_externa=pap.acaid ";
$RS2 = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS2);
if ($nlinhas >= 0)
{
   for ($i=0; $i<=$nlinhas;$i++)
   {
     $res = $db->carrega_registro($RS2,$i);
    // a linha abaixo transforma em variáveis todos os campos do array
     if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
          $fusao .= $acao.' ';
    }
  }
  }

?>
        <td colspan="2"><?=campo_textarea('fusao','N','N','',80,5,'','','','');?></td>
      </tr>
  <?if ($db->verifica_momento())
  {
   if (! $_REQUEST['acaid']){?>
      
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Incluir' onclick="gravar_janela('I')"></tr>
      <? } else if ($db->ehcriador($_REQUEST['acaid'])){?>
      <tr ><td colspan="2" align="center"><input type='button' class="botao" value='Alterar' onclick="gravar_janela('A')"></tr>      
      <?}
  }?>
    </table>
  
    
</form>  
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="2" valign="top" align="center" class="title"><strong>Novas ações propostas</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Proposta (Programa - Ação - Título da ação)</strong></td>     
    </tr>
</thead>
<tbody>
<?
$sql= "select a.acacod as codigo, a.prgcod||'-'||a.acacod||'-'||a.acadsc as descricao, a.prgcod, a.acaid from elabrev.ppaacao_proposta a  where a.usucpf='".$_SESSION['usucpf']."' and prsano='".$_SESSION['exercicio']."'  order by a.acacod,descricao";

$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
       	 print "<tr class='$marcado'>
                <td><img border='0' src='../imagens/alterar.gif' title='Alterar a proposta.' onclick=".'"altera_acao('."'$acaid')".'"'.">";
       	 if ($db->verifica_momento())
       	 print "&nbsp;&nbsp;&nbsp;<img border='0' src='../imagens/excluir.gif' title='Excluir a proposta.' onclick=".'"excluir_acao('."$acaid,'$descricao')".'">';
       	 
       	 print "</td><td>$descricao</td></tr>";
  		 $i++;
		}
	}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de novas ações</td></tr>';
}
?>
</tbody>
    </table>
    

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
    <td colspan="3" valign="top" align="center" class="title"><strong>Novas ações propostas por outros Gestores</strong></td>
    </tr>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Proposta (Programa - Ação - Título da ação)</strong></td>        <td valign="top" class="title"><strong>Proponente</strong></td>
      
    </tr>
</thead>
<tbody>
<?
$sql= "select a.acacod as codigo, a.prgcod||'-'||a.acacod||'-'||a.acadsc as descricao, a.prgcod, a.acaid, 

case when a.usucpf !='".$_SESSION['usucpf']."'  then '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || a.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum ||'-'||un.unidsc else u.usunome||'-'||u.usufoneddd||'-'||u.usufonenum||'-'||un.unidsc end as autor 

from elabrev.ppaacao_proposta a  inner join seguranca.usuario u on u.usucpf=a.usucpf inner join unidade un on un.unicod=u.unicod where a.usucpf not in ('".$_SESSION['usucpf']."') and prsano='".$_SESSION['exercicio']."'  order by a.prgcod,a.acacod,descricao";


$rs = $db->carregar( $sql );
if (  $rs && count($rs) > 0 )
	{
	 $i=0;
	 foreach ( $rs as $linha )
		{
		 foreach($linha as $k=>$v) ${$k}=$v;
		 if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
       	 print "<tr class='$marcado'>
  <td><img border='0' src='../imagens/alterar.gif' title='Alterar a proposta.' onclick=".'"ver_acao('."'$acaid')".'"'."></td><td>$descricao</td><td>$autor</td></tr>";
  		 $i++;
		}
	}
else
{
  print '<tr class="'.$marcado.'"><td></td><td>Não há registros de novas ações</td></tr>';
}


?>
</tbody>
    </table>   
    
    

<script>

   function gravar_janela(cod)
   {	
   	  //prepara_formulario();
   	  // testar todos os campos antes de incluir

   	  if (!validaBranco(document.formulario.unicod, 'Unidade Orçamentária')) return;
   	  if (!validaBranco(document.formulario.prgid, 'Programa')) return;
   	  if (!validaBranco(document.formulario.esfcod, 'Esfera')) return;
   	  if (!validaBranco(document.formulario.funcod, 'Função')) return;
   	  if (!validaBranco(document.formulario.sfucod, 'Subfunção')) return;
   	  if (!validaBranco(document.formulario.acadsc, 'Título')) return;
   	  if (!validaBranco(document.formulario.acafinalidade, 'Finalidade')) return;
   	  if (!validaBranco(document.formulario.acadescricao, 'Descrição')) return;
   	  if (!validaBranco(document.formulario.procod, 'Produto')) return;
   	  if (!validaBranco(document.formulario.unmcod, 'Unidade de medida')) return;  
  	  
   	  if ( !validar_radio( document.formulario.acasnmetanaocumulativa, 'Cumulatividade da meta' ) ) {
			alert ('Cumulatividade da meta!');
			document.formulario.acasnmetanaocumulativa.focus();
			return;
		} 
	  
   	  if (!validaBranco(document.formulario.taccod, 'Tipo de ação')) return; 
   	  if (document.formulario.taccod.value==1)     	     	  
   	  { // é projeto
   	  	if (document.formulario.acamesinicio.value <0 ||  document.formulario.acamesinicio.value >12)
   	  	{
   	  		alert ('Mês de início inválido!');
			document.formulario.acamesinicio.focus();
			return;
   	  	}
   	  	if (document.formulario.acamestermino.value <0 ||  document.formulario.acamestermino.value >12)
   	  	{
   	  		alert ('Mês de término inválido!');
			document.formulario.acamestermino.focus();
			return;
   	  	}   	  	
   	  	if (!validaBranco(document.formulario.acamesinicio, 'Mês de início do projeto')) return; 
   	  	if (!validaBranco(document.formulario.acaanoinicio, 'Ano de início do projeto')) return; 
   	  	if (!validaBranco(document.formulario.acamestermino, 'Mês de término do projeto')) return; 
   	  	if (!validaBranco(document.formulario.acaanotermino, 'Ano de término do projeto')) return;    
   	  	if (document.formulario.acaanotermino.value < document.formulario.acaanoinicio.value)	  	{
   	  		alert ('Ano de término do projeto não pode ser anterior ao de início');
   	  		return;
   	  	}
   	  	else if (document.formulario.acaanotermino.value == document.formulario.acaanoinicio.value && document.formulario.acamestermino.value < document.formulario.acamesinicio.value)
   	  	{
   	  		alert ('Mês de término do projeto não pode ser anterior ao de início');
   	  		document.formulario.acamestermino.focus();
   	  		return;   	  		
   	  	}
   	  	if (!validaBranco(document.formulario.acavlrcustototal, 'Custo estimado do projeto')) return;       	  	
   	  }
 	  
   	  if ( !validar_radio( document.formulario.acasnoutrasfontes, 'Quanto ao orçamento' ) ) 		{
			alert ('Quanto ao Orçamento!');
			document.formulario.acasnoutrasfontes.focus();
			return;
		}
		if (document.formulario.acasnoutrasfontes[0].checked)
		{	
			// é orçamentário
				   
			if ( !validar_radio( document.formulario.acaoorcamento, 'Tipo de orçamento' ) ) 			{
				alert ('Tipo de Orçamento!');
				document.formulario.acaoorcamento.focus();
				return;
			}
			if (document.formulario.acatransfer.checked)
			{			
	           if ( !validar_radio( document.formulario.formaimplementacao, 'Escolha um tipo de transferência.' ) ) 			{
				alert ('Escolha um tipo de transferência!');
				document.formulario.formaimplementacao.focus();
				return;
			}											
			}
 
			if (!validaBranco(document.formulario.acadetalhamento, 'Detalhamento da Implementação')) return;
		
		}

		if (document.formulario.acasnoutrasfontes[1].checked)
		{
			if (!validaBranco(document.formulario.acavlrcustototal, 'Custo estimado da ação')) return; 
		}
	
   	  	if (!validaBranco(document.formulario.acabaselegal, 'Base legal')) return;
   	  			
   	  if (cod=='I') document.formulario.acao_n.value='inclui';
   	  else document.formulario.acao_n.value='altera';
   	   	   document.formulario.submit();
   }


   function busca_unidade()
   {
   	document.formulario.submit();
   }
   
  function altera_acao(cod) {
     document.formulario.acaid.value = cod;
     document.formulario.submit();
  }
  
  function ver_acao(cod) {
     document.formulario.veracaid.value = cod;
     document.formulario.submit();
  }  
  
    function excluir_acao(cod,descricao)
   {
   	if( window.confirm( "Confirma a exclusão da Ação "+descricao+" ?" ))
    {
   	   	  document.formulario.acao_n.value='exclui';
   	  document.formulario.acaid.value=cod;
   	   document.formulario.submit();
    }
   }  
  
  function popfun(acaid)
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/propostafusao_inclusao&acao=A&acaid='+acaid;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }
  function geraclone()
  {
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/geraclone&acao=A';
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=250");
  }  
  function incluir_unid(cod)
  {

	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/acao/inclui_unid&acao=A&acaid='+cod;
		window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=500,height=450");
  }

  function tipoacao(cod)
  {
	document.formulario.submit();
  }
  function tipoorcamento(cod)
  {
	document.formulario.submit();
  }  
  function escolhe_gestora(cod)
  {
	document.formulario.submit();
  }  
    
</script>

