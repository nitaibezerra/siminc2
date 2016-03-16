<?php
	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: unidade.inc
		Finalidade: controlar os valores limites da proposta por Montante Categoria Econômica e Montante por Coluna
	
		Status da Proposta
		A - Ativo
		H - Hipótese

	*/

$idproposta = $_REQUEST['idproposta'];
$mostraSub = 0;
$mostraTotal = 0;
$clausula = '';
$checar = '';
$ligaTabela = '';
$codColuna = $_REQUEST['codColuna'];
$codMatriz = $_REQUEST['codMatriz'];
$qtdFracao = 0;
$qtdLimiteFonte = 0;
$categUltimo = 0;
//$tppid = (integer) $_REQUEST['tppid'];
// força tipo ser Proposta
$tppid = 1;

/*
Ação para atualizar o Status da proposta
Condições: Verifica o Ano da proposta, 
		   não permite ativar mais de uma proposta no mesmo ano
*/
if ($_REQUEST['acao_n']=='AtivarProposta')
{
	$qtdAtivada = 0;
	$ano = "";
	$sql = "";

	$ano = $db->pegaUm( "Select ppoano from elabrev.propostaorcamento where ppoid = " . $idproposta );
	$qtdAtivada=$db->pegaUm( "Select count(ppodsc) from elabrev.propostaorcamento where ppoano = '" . $ano . "' and ppostatus = 'A'" );
	if ( $qtdAtivada == 0 )
	{

		$sql = "update elabrev.propostaorcamento set ppoStatus = 'A' where ppoid = " . $idproposta;
	   	if ( $db->executar( $sql ) )	
	   	{
   		    $db->commit();
			?>
				<script>
						alert('Proposta Ativada!');
						location.href='elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=I&idproposta=<?=$idproposta?>';
				</script>
			<?php
			exit();
	   	}

	}
	else 
	{
		$sql="update elabrev.propostaorcamento set ppoStatus = 'H' where ppoano = '" . $ano . "'";
	   	if ( $db->executar( $sql ) )	
	   	{
			$sql="update elabrev.propostaorcamento set ppoStatus = 'A' where ppoid = " . $idproposta;
			$db->executar( $sql );
		   	$db->commit();
			?>
				<script>
						alert( 'Proposta Ativada!' );
						location.href='elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=I&idproposta=<?= $idproposta ?>';
				</script>
			<?
			exit();
	   	}
		
	}
}

/*
Função para gerar cópia da proposta e suas tabelas filhas (Tabelas: limiteppo, montantematriz, fracao, limitefonte)
*/
if ( $_REQUEST['acao_n'] == 'GerarCopia' ) 
{
	
	
	$descricao=$db->pegaUm( "Select ppodsc from elabrev.propostaorcamento where ppoid = ".$idproposta);
	$ano=$db->pegaUm( "Select ppoano from elabrev.propostaorcamento where ppoid = ".$idproposta);
	$exercicio=$db->pegaUm( "Select ppoanoexercicio from elabrev.propostaorcamento where ppoid = ".$idproposta);
		
   	$sql="insert into elabrev.propostaorcamento (ppodsc,ppoano,ppostatus,ppoanoexercicio,tppid) values ('Cópia de " .$descricao. "','" .$ano. "','H','" .$exercicio. "', " . $tppid . ")";
   	if ($db->executar( $sql ))	
   	{
		$id=(integer)$db->pegaUm( "select currval( 'elabrev.propostaorcamento_ppoid_seq'::regclass ) as ppoid");
		if ($id == 0)
		    $db->rollback();
		else 
		{

			$sql = "select * from elabrev.limiteppo where ppoid = ".$idproposta." ";	
		    $RS = $db->record_set($sql);
	        $nlinhas = $db->conta_linhas($RS);
		    $RS = $db->carregar($sql);
	        if ($nlinhas >= 0)
	        {
				foreach ($RS as $linha)
				{

					$sql2="insert into elabrev.limiteppo (ctecod,ppoid,lmpvalor) values (" .$linha["ctecod"]. "," .$id. "," .$linha["lmpvalor"]. ")";
					$db->executar( $sql2 );
				}
	        }

	        $sql = "select * from elabrev.montantematriz where ppoid = ".$idproposta." ";	
		    $RS = $db->record_set($sql);
	        $nlinhas = $db->conta_linhas($RS);
		    $RS = $db->carregar($sql);
	        if ($nlinhas >= 0)
	        {
				foreach ($RS as $linha)
				{
					$sql3="insert into elabrev.montantematriz (mtrid,ppoid,mtmvlrlimite,mtmformatofracao,mtmano) values (" .$linha["mtrid"]. "," .$id. "," .$linha["mtmvlrlimite"]. ",'I','".$linha["mtmano"]."')";
					$db->executar( $sql3 );
					$idMontante=(integer)$db->pegaUm( "select currval( 'elabrev.montantematriz_mtmid_seq'::regclass ) as mtmid");

					$sql2 = "select * from elabrev.fracao where mtmid = ".$linha["mtmid"]." ";	
					$RS2 = $db->record_set($sql2);
			        $nlinhas2 = $db->conta_linhas($RS2);
				    $RS2 = $db->carregar($sql2);
			        if ($nlinhas2 >= 0)
			        {
						foreach ($RS2 as $linha2)
						{
							$sql = "insert into elabrev.fracao ( mtmid, unicod, unitpocod, fravalor ) values( " . $idMontante . ", '" . $linha2["unicod"] . "', '" . $linha2["unitpocod"] . "', " . $linha2["fravalor"] . " )";
							$db->executar( $sql );
						}
				
			        }				
				
			        $sql4 = "select * from elabrev.limitefonte where mtmid = ".$linha["mtmid"]." ";	
				    $RS4 = $db->record_set($sql4);
			        $nlinhas4 = $db->conta_linhas($RS4);
				    $RS4 = $db->carregar($sql4);
			        if ($nlinhas4 >= 0)
			        {
						foreach ($RS4 as $linha4)
						{
							$sql = "insert into elabrev.limitefonte ( foncod, unicod, unitpocod, mtmid, lmfvalor ) values( " . $linha4["foncod"] . " , " . $linha4["unicod"] . ", '" . $linha4["unitpocod"] . "', '" . $idMontante . "', " . $linha4["lmfvalor"] . " )";
							$db->executar( $sql );
						}
				
			        }				

				}
//				dbg(1,1);
	        }
		
		}			
			$db->commit();
		   $db->sucesso($modulo,'&idproposta='.$id.'');
   	}

}



/*
Condição para excluir uma coluna
*/
if ($_REQUEST['acao_n']=='excluirColuna') {
    $qtdFracao = (integer)$db->pegaUm("select COUNT(fravalor) as contador from fracao where mtmid = ".$codColuna."");
    $qtdLimiteFonte = (integer)$db->pegaUm("select SUM(lmfvalor) as contador from limitefonte where mtmid = ".$codColuna."");

    if ($qtdFracao > 0 || $qtdLimiteFonte > 0) { ?>
<script type="text/javascript">
        alert( 'Não foi possível remover, pois existem valores distribuídos para a coluna!' );
        window.location.href='elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=I&idproposta='+<?= $idproposta ?>;
</script>
<?php
        exit();
    } else {
        if ($codColuna != 0) {
            $sql2 = "DELETE FROM elabrev.limitefonte WHERE mtmid = {$codColuna}";
            $db->executar($sql2);

            $sql2="delete from elabrev.montantematriz where mtmid = " . $codColuna;
            $db->executar($sql2);
        }
        if ($codMatriz != 0) {
            // -- Apaga as entradas daquela matriz relacionada à coluna excluída
            $sql3="delete from elabrev.montantematriz where mtrid = {$codMatriz} AND mtmid = {$codColuna}";
            $db->executar($sql3);

            // -- Verificando a existência de outras colunas antes de apagar as informações da matriz. Verificação
            // -- necessária, pois um mesmo item de matriz pode estar associado a mais de uma proposta. Logo,
            // -- a matriz só será excluída, se nenhuma proposta fizer uso dela.
            $sql3 = "SELECT COUNT(1) AS qtdmatriz_com_montante FROM elabrev.montantematriz WHERE mtrid = {$codMatriz}";
            if (0 == $db->pegaUm($sql3)) {
                $sql3="delete from elabrev.unidadematriz where mtrid = " . $codMatriz;
                $db->executar($sql3);
                $sql3="delete from elabrev.matrizgnd where mtrid = " . $codMatriz;
                $db->executar($sql3);
                $sql3="delete from elabrev.fonterecursomatriz where mtrid = " . $codMatriz;
                $db->executar($sql3);
                $sql3="delete from elabrev.matriz where mtrid = " . $codMatriz;
                $db->executar($sql3);
            }
        }
        $db->commit(); ?>
<script type="text/javascript">
    alert( 'Operação Realizada com Sucesso!' );
    window.location.href='elabrev.php?modulo=principal/propostaorcamentaria/montante&acao=I&idproposta=<?= $idproposta ?>'; 
</script>
<?
        exit();
    }
}


/*
Quando existir alguma proposta monta a claúsula para mostrar só os grupos de matrizes existentes para a proposta
$idproposta = id da proposta orcamentaria
*/
/*
Exibi sempre os grupos de colunas disponíveis
09-Julho-07 por Renan
*/
if ( $idproposta != '' )
{
	/*
    $sql = "select distinct a.gpmid from elabrev.grupomatriz a inner join elabrev.matriz b on b.gpmid = a.gpmid inner join elabrev.montantematriz c on c.mtrid = b.mtrid where c.ppoid = ".$idproposta;
    $RS = $db->record_set( $sql );
    $nlinhas = $db->conta_linhas( $RS );
    if ( $nlinhas >= 0 )
	{
		for ( $i = 0; $i <= $nlinhas; $i++ )
		{
		  	if( is_array( $res ) ) extract( $res );
			if ( $i == 0 )
			{
				$clausulaGrupo = "where gpmid in ( " . $gpmid;
			}
			else
			{ 
				$clausulaGrupo .= "," . $gpmid;
			}
		}
		$clausulaGrupo .= ")";
		dbg( $clausulaGrupo );
	}
	else
	{
		$clausulaGrupo = "where gpmid = 0 ";	
	}
	*/
	$clausulaGrupo = "";
}
else 
{
	$clausulaGrupo = "";
}

	
/*
Condição de inclusão e alteração da proposta
*/
if ($_REQUEST['acao_n']=='inclui' or $_REQUEST['acao_n']=='altera')
{
	
	// mandou gravar
	$descricao = $_REQUEST['ppodsc'];
	//$ano = $_REQUEST['ppoano'];
	$ano = $_SESSION['exercicio'] + 1;
	$anoexercicio = $_REQUEST['ppoanoexercicio'];
//	$data = $_REQUEST[''];
	$valorCat = $_REQUEST['lmpvalor'];
	$idLimiteppo = $_REQUEST['lmpid'];
	$idCat = $_REQUEST['ctecod'];
	$id = 0;

	
/*
Condição de Alteração
*/
	if ($_REQUEST['acao_n']=='altera') 
    {
    	$sql="update elabrev.propostaorcamento set tppid = " . $tppid . ",  ppodsc='" . $descricao . "', ppoano='" . $ano . "' where ppoid= " . $idproposta;
	    $res = $db->executar($sql);	
	    
		for ($a=0; $a<=(count($valorCat)-2);$a++)
		{
			$vlCat = $valorCat[$a];
			$vlCat = str_replace('.','',$vlCat);
			$vlCat = str_replace(',','.',$vlCat);
			// existem propostas que não contem limites para categorias economicas novas
			if ( $idLimiteppo[$a] )
			{
				$sql2 = "update elabrev.limiteppo set ctecod = " .$idCat[$a]. ", ppoid = " .$idproposta. ", lmpvalor = " .$vlCat. " where lmpid = ".$idLimiteppo[$a];
			}
			else
			{
				$sql2 = "insert into elabrev.limiteppo ( ctecod, ppoid, lmpvalor ) values ( " . $idCat[$a] . ", " . $idproposta . ", " . $vlCat . " )";
			}
		    $res = $db->executar( $sql2 );	
		}
    
		//$sql = "select gpmid, gpmdsc from grupomatriz ".$clausulaGrupo." order by gpmordem";
	    
	    $sql = "select g.gpmid, g.gpmdsc from elabrev.grupomatriz g inner join elabrev.matriz m ON m.gpmid = g.gpmid inner join elabrev.montantematriz mm ON mm.mtrid = m.mtrid where m.mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "' " . $clausulaGrupo . " group by g.gpmid, g.gpmdsc, g.gpmordem order by g.gpmordem"; 
			
		$RS = $db->carregar($sql);
		foreach ($RS as $linha)
		{
			$montanteAlterado = $_REQUEST['montante_alterado_'.$linha["gpmid"]];
			$valorCol = $_REQUEST['mtmvlrlimite'.$linha["gpmid"].'_'];
			$idMatriz = $_REQUEST['mtrid_'.$linha['gpmid']];
			$idMontante = $_REQUEST['mtmid_'.$linha['gpmid']];
			
			for ($a=0; $a<=(count($valorCol)-2);$a++)
			{
				// Só atualiza os registros que foram alterados
				if($montanteAlterado[$a] == "1") {
					if ($valorCol[$a] != '') 
					{
						$vlCol = $valorCol[$a];
						$vlCol = str_replace('.','',$vlCol);
						$vlCol = str_replace(',','.',$vlCol);
					}
					else
						$vlCol = 0.00;
					
					// existem propostas que não contem limites para categorias economicas novas
					// isso acontece pq os limites são cadastrados primeiros, e só depois as matrizes são preenchidas
					if ( $idMontante[$a] )
					{ 
						$sql3 = 
							"update elabrev.montantematriz
							set
								mtrid = " . $idMatriz[$a] . ",
								ppoid = " . $idproposta . ",
								mtmvlrlimite = " . $vlCol . ",
								mtmformatofracao = 'I',
								mtmano = '" . $ano . "'
							where
								mtmid = " . $idMontante[$a];
					}
					else
					{
						$sql3 =
							"insert into elabrev.montantematriz
							( mtrid, ppoid, mtmvlrlimite, mtmformatofracao, mtmano ) values
							( " . $idMatriz[$a] . "," . $idproposta . "," . $vlCol . ",'I','" . $ano . "')";
						
					}
				    $res = $db->executar($sql3);
				}
			}				
		}
	    $db->commit();
	   $db->sucesso($modulo,'&idproposta='.$idproposta.'');
    
    }

/*
Condição de inclusão da proposta
*/
    else 
    {

    	$sql="insert into elabrev.propostaorcamento (ppodsc,ppoano,ppostatus,ppoanoexercicio,tppid) values ('" .$descricao. "','" .$ano. "','H','" .$_SESSION['exercicio']. "', " . $tppid . ")";

    	if ($db->executar( $sql ))	
    	{

			$id=(integer)$db->pegaUm( "select currval( 'elabrev.propostaorcamento_ppoid_seq'::regclass ) as ppoid");
	
			if ($id == 0)
				    $db->rollback();
			else 
			{
				for ($a=0; $a<=(count($valorCat)-2);$a++)
				{
					$vlCat = $valorCat[$a];
					$vlCat = str_replace('.','',$vlCat);
					$vlCat = str_replace(',','.',$vlCat);
					$vlCat = $vlCat ? $vlCat : 0;
					$sql2="insert into elabrev.limiteppo (ctecod,ppoid,lmpvalor) values (" .$idCat[$a]. "," .$id. "," .$vlCat. ")";
//					dbg($sql2);
				    $res = $db->executar($sql2);

				}
				
				$sql = "select gpmid, gpmdsc from grupomatriz order by gpmordem";	
			    $RS = $db->carregar($sql);
				foreach ($RS as $linha)
				{
					$valorCol = $_REQUEST['mtmvlrlimite'.$linha["gpmid"].'_'];
					$idMatriz = $_REQUEST['mtrid_'.$linha['gpmid']];
					
					for ($a=0; $a<=(count($valorCol)-2);$a++)
					{

						if (isset($_REQUEST['mtrid_check_'.$linha['gpmid']] [$a]))
						{
							$vlCol = $valorCol[$a];
							if ($vlCol == '')
								$vlCol = 0.00;
							else 
							{
								$vlCol = str_replace('.','',$vlCol);
								$vlCol = str_replace(',','.',$vlCol);
							}

							$sql3="insert into elabrev.montantematriz (mtrid,ppoid,mtmvlrlimite,mtmformatofracao,mtmano) values (" .$idMatriz[$a]. "," .$id. "," .$vlCol. ",'I','".$ano."')";
//							dbg($sql3);
						    $res = $db->executar($sql3);	
						}
					}

				}					
					
//			  dbg(1,1);
			    
			    $db->commit();
			    $db->sucesso($modulo,'&idproposta='.$id.'');    		

			}
		
    
    	}

	}


}


/*
Exibição dos dados da proposta, carrega os dados
*/
if ($_REQUEST['acao_n']=='exibir' || $_REQUEST['idproposta']!='')
{
    $sql = "select * from elabrev.propostaorcamento where ppoid=$idproposta";
//    dbg($sql,1);
	$saida = $db->recuperar($sql);
     if(is_array($saida)) {foreach($saida as $k=>$v) ${$k}=$v;}

}

include  APPRAIZ."includes/cabecalho.inc";
print "<br>";
$db->cria_aba($abacod_tela,$url,'');
if ( $_REQUEST['acao'] == 'A' )
{
	monta_titulo( 'Alterar Proposta' , '' );
}
else
{
	monta_titulo( 'Inserir Proposta' , '' );
}

if ($idproposta != '')
{
	$PropostaAtivaAtual=$db->pegaUm( "Select count(ppodsc) from elabrev.propostaorcamento where ppoano = '".$ppoano."' and ppoid = ".$idproposta." and ppostatus = 'A'");
	$qtdAtiva=$db->pegaUm( "Select count(ppodsc) from elabrev.propostaorcamento where ppoano = '".$ppoano."' and ppostatus = 'A'");
	if ($qtdAtiva != 0)
	$NomeAtiva=$db->pegaUm( "Select ppodsc from elabrev.propostaorcamento where ppoano = '".$ppoano."' and ppostatus = 'A'");
}

?>
<div align="center">
<form method="POST"  name="formulario">
<input type=hidden name="acao_n">
<input type=hidden name="idproposta" value=<?=$_REQUEST['idproposta']?>>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value=0>
<input type=hidden name="codColuna" value="">
<input type=hidden name="codMatriz" value="">
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="SubTituloDireita" >Descrição</td>
        <td >
		<?=campo_texto('ppodsc','S','','',120,100,'','');?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita" >Ano proposta</td>
        <td >
        	<? $ppoano = $_SESSION['exercicio'] + 1; ?>
			<?= campo_texto( 'ppoano', 'S', 'N', '', 6, 4, '', '' ); ?>
		</td>
      </tr>
		<tr>
			<td align='right' class="SubTituloDireita" >Tipo</td>
			<td>
				<?php
					//$sqlTpoProposta = 'select tppid as codigo, tppdsc as descricao from elabrev.tipoproposta where tppstatus = \'A\'';
					//$db->monta_combo( 'tppid', $sqlTpoProposta, 'S', '', '', '' );
				?>
				<input type="hidden" name="tppid" value="<?= $tppid ?>"/>
				Proposta
			</td>
		</tr>
	  <?php if ( $idproposta ) : ?>
      <tr>
        <td align='right' class="SubTituloEsquerda">&nbsp;&nbsp;</td>
        <td>
             <input type='button' class="botao" value='Comentários' onclick="InsereComentarios(<?=$idproposta?>)">
		</td>
      </tr>
      <?php endif; ?>
</table>

<?= monta_titulo( 'Montante por Cat. Econômica', '*Caso não seja informado o limite, o mesmo será calculado de acordo com o somatório da distribuição por unidade' ); ?>


<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
    	<td>Categoria Econômica</td>
    	<td>Montante (Em R$)</td>
    	<td width="24%">&nbsp;&nbsp;</td>
    </tr>
    <?
	// Claúsula que deve ser inserida de acordo com uma proposta exsitente ou uma nova
    if ( is_null( $idproposta ) )
    	$clausula = " and b.ppoid = 0 ";
    else
    	$clausula = " and b.ppoid = " . $idproposta;
	// Executa consulta p/ trazer as matrizes da proposta
    $sql = "
		select
			a.ctecod,
			a.ctedsc,
			b.lmpvalor as lmpvalor,
			b.lmpid
		from categoriaeconomica a
			left join elabrev.limiteppo b on
				b.ctecod = a.ctecod
  				" . $clausula. "
		where ctestatus='A'";
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
	if ($nlinhas<0) print "</table><font color='red'>Não foram encontrados Registros</font>";
	else {
		$mostraSubCat = 0;
		$conta = 0;
		for ($i=0; $i<=$nlinhas;$i++) {
		$conta = $conta + 1;
		$res = $db->carrega_registro($RS,$i);
	  	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

//Carrega os dados num array que serão exibidos
	  	$var = "lmpvalor[$i]";
		if (is_null($lmpvalor))
	  	${$var} = '';
		else
	  	${$var} = formata_valor(floatval($lmpvalor),0);

		$mostraSubCat += floatval($lmpvalor);
	?>
     <tr>
    	<td>
    		<?= $ctedsc; ?>
    	</td>
    	<td>
    		<input type="hidden" name="ctecod[<?=$i?>]" value="<?=$ctecod;?>">
    		<input type="hidden" name="lmpid[<?=$i?>]" value="<?=$lmpid;?>">
 		   	<?=campo_texto($var,'S',$habil,'',20,18,'','','right','',0,'onBlur="MouseBlur(this);" onkeyup="this.value=formata_decimal2(this.value);SomaValores('.$i.',\'lmpvalor\',\'conta1\',0,\'\',null,'.$i.',1);"');?>
    		<input type="hidden" name="obriga1[<?=$i?>]" value="<?=${$var};?>">
    	</td>
    	<td>
    		&nbsp;&nbsp;
    	</td>
    </tr>   
	<?}
    	$var = "lmpvalor[$i]";
		${$var} = formata_valor(floatval($mostraSubCat),0);
		$categUltimo = $i;
	}?>
   	    	<input type="hidden" name="contador1" value="<?=$i;?>">
	<tr>
    	<input type="hidden" name="conta1" value="<?=$conta;?>">
    	<td align="right">Total:</td>
    	<td><?=campo_texto("lmpvalor[$i]",'N','N','',20,18,'','','right');?></td>
    	<td>&nbsp;&nbsp;</td>
    </tr> 

</table>

		<?php if ( $idproposta ) : ?>
			<?= monta_titulo( 'Montante por Coluna', '*Caso não seja informado o limite, o mesmo será calculado de acordo com o somatório da distribuição por unidade' ); ?>
		<?php endif; ?>
		
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"  align="center">
		<?php if ( $idproposta ) : ?>
		    <tr>
		    	<td>Colunas</td>
		    	<td>Montante (Em R$)</td>
				<?php if ( $idproposta != '' ) : ?>
					<td>Limite (Em R$)</td>
				<?php endif; ?>
		    </tr>
		    <?php
			//Seleciona somente os grupos de matrizes que foram gravados para a proposta, caso contrário mostra todas
		  //  $sql = "select gpmid, gpmdsc from elabrev.grupomatriz " . $clausulaGrupo . " order by gpmordem";
		    $sql = "select g.gpmid, g.gpmdsc from elabrev.grupomatriz g inner join elabrev.matriz m ON m.gpmid = g.gpmid inner join elabrev.montantematriz mm ON mm.mtrid = m.mtrid where m.mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "' " . $clausulaGrupo . " group by g.gpmid, g.gpmdsc, g.gpmordem order by g.gpmordem"; 
			$RS = $db->record_set( $sql );
		    $nlinhas = $db->conta_linhas( $RS );
			if ( $nlinhas < 0 )
			{
				print "
					</table>
					<font color='red'>Não foram encontrados Registros</font>
					<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"  align=\"center\">
				";
			}
			else
			{
				$z = 0;
				for ( $i=0; $i <= $nlinhas; $i++ )
				{
			  		$res = $db->carrega_registro( $RS, $i );
			  		if( is_array( $res ) ) extract( $res );
					?>
						<tr>
			    			<td style="cursor:pointer;" onclick="javascript:onOffBloco( 'grupo_<?=$gpmid?>' );">
			    				<img border="0" src="/imagens/mais.gif" id="grupo_<?=$gpmid?>_img"/>&nbsp;<b><?=$gpmdsc;?></b>
			    			</td>
		    				<!-- <td>&nbsp;&nbsp;</td>  -->
							<? if ( $idproposta != '' ) : ?>
								<td>&nbsp;&nbsp;</td>
							<? endif; ?>
						</tr>
					<?
					//Caso esteja exibindo uma proposta joga numa variável o valor "inner" juntamente com o ID da proposta, para trazer as matrizes gravadas, 
					//caso contrário, carrega todas usando o valor "left" na ligação da tabela e o valor 0 pois não irá trazer nenhum valor
		    		if ( is_null( $idproposta ) )
		    		{
			    		$clausula = " and b.ppoid = 0 "; 
						$ligaTabela = " left ";
		    		} 
		    		else {
                                    $clausula = " and b.ppoid = " . $idproposta . " ";
                                    $ligaTabela = " inner ";
		    		}
					$sql2 =
						"select
							a.mtrid,
							a.mtrdsc,
							b.mtmvlrlimite as mtmvlrlimite,
							b.mtmid,
							a.mtrmatriz,
			    			Case a.mtrmatriz
								when 't' then SUM ( coalesce ( (c.lmfvalor*b.mtmvlrlimite)/100, 0) )
								else SUM ( coalesce ( c.lmfvalor, 0) ) end as distribuido
						from elabrev.matriz a
							" . $ligaTabela . " join elabrev.montantematriz b on
								b.mtrid = a.mtrid
								" . $clausula . " 
							left join elabrev.limitefonte c on
								c.mtmid = b.mtmid 
						where
							a.gpmid = " . $gpmid . " and
							mtrano = '" . ( $_SESSION['exercicio'] + 1 ) . "'
						group by 
							a.mtrid,
							a.mtrdsc,
							b.mtmvlrlimite,
							b.mtmid,
							a.mtrmatriz
						order by
							a.mtrdsc";
//                                        ver($sql2, d);
				    $RS2 = $db->record_set( $sql2 );
				    $nlinhas2 = $db->conta_linhas( $RS2 );
					if ( $nlinhas2 >= 0 ) 
					{
						$mostraSub = 0;
						$conta = 0;
						$mostraSubDistribuido = 0;
						for ( $a = 0; $a <= $nlinhas2; $a++ )
						{
							$conta = $conta + 1;
					  		$res2 = $db->carrega_registro( $RS2, $a );
					  		if( is_array( $res2 ) ) extract( $res2 );
						  	$var = "mtmvlrlimite" . $gpmid . "_[$a]";
					  		if ( is_null( $mtmvlrlimite ) || $mtmvlrlimite == '0.00' )
					  		{
					  			${$var} = '';
					  		}
							else
							{
					  			${$var} = formata_valor(floatval($mtmvlrlimite),0);
							}
							
					  		$var2 = "vlDistribuido" . $gpmid . "_[$a]";
							${$var2} = formata_valor( floatval( $distribuido ), 0 );
							
							$mostraSub += floatval( $mtmvlrlimite );
							$mostraSubDistribuido += floatval( $distribuido );
							$mostraTotal += floatval( $mtmvlrlimite );
							$mostraTotalDistribuido += floatval( $distribuido );
							$mtrmatriz = $mtrmatriz == 't' ? 'S' : 'N';
							$checar = is_null( $mtmid ) ? '' : 'checked';
							
							//$a % 2 ? $cor = "#dedfde" : $cor = "";
							?>
						     <tr name="grupo_<?=$gpmid?>_td[]" style="display:none;" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
							    	<td>
								    	<?php if ( $idproposta != '' ) : ?>
											<img src="../imagens/excluir.gif" title="Excluir a matriz da Proposta" border="0" align="absmiddle" onclick="ExcluirColuna(<?=($mtmid ? $mtmid : 0)?>,'<?=$mtrdsc?>', <?=($mtrid ? $mtrid : 0)?>);">
											&nbsp;
											<?php if ( $mtrmatriz == 'S' ) : ?>
												<img src="../imagens/money.gif" border="0" title="Distribuir valores fracionados para a matriz" align="absmiddle" onclick="AbrirFracao('<?=urlencode(md5_encrypt($mtmid,''))?>','<?=${$var}?>','<?=$mtrdsc?>','<?=$var?>');">
											<?php else : ?>
												<img src="../imagens/money.gif" border="0" title="Distribuir valores para a matriz" align="absmiddle" onclick="AbrirLimite('<?=urlencode(md5_encrypt($mtmid,''))?>','<?=${$var}?>','<?=$mtrdsc?>','<?=$var?>');">
											<? endif; ?>
										<?php else : ?>
							    			<input type="checkbox" name="mtrid_check_<?=$gpmid?>[<?=$a?>]" <?=$checar?> value="<?=$mtrid;?>" onclick="InseriuValor('mtrid_check_<?=$gpmid?>',<?=$a?>,<?=$gpmid?>,'conta<?=$gpmid?>');">
							    		<?php endif; ?>
							    		&nbsp;&nbsp;&nbsp; <?=$mtrdsc;?>
						    			&nbsp;
						    		</td>
							    	<td>
							    		<input type="hidden" name="mtrid_<?=$gpmid?>[<?=$a?>]" value="<?=$mtrid;?>">
							   	    	<input type="hidden" name="mtmid_<?=$gpmid?>[<?=$a?>]" value="<?=$mtmid;?>">
										<?php 
											$obriga = 'null';
											if ( $mtrmatriz == 'S' )
											{
												?>
										   	    	<input type="hidden" name="obriga2[<?=$z?>]" value="<?=${$var};?>">
										   	    	<input type="hidden" name="grupo[<?=$z?>]" value="<?=$gpmid;?>">
										   	    	<input type="hidden" name="indice[<?=$z?>]" value="<?=$a;?>">
												<? 
											$obriga = $z;
											$z = $z+1;
										}
									?>
									<?
										if ( $_REQUEST['acao'] == 'A' ) {
											echo campo_texto( $var,$mtrmatriz,$habil,'',20,18,'','','right','',0,'onBlur="MouseBlur(this);" onkeypress="marcarMontanteAlterado(\'montante_alterado_'.$gpmid.'['.$a.']\');" onkeyup="this.value=formata_decimal2(this.value);SomaValores('.$a.',\'mtmvlrlimite'.$gpmid.'_\',\'conta'.$gpmid.'\',1,\'mtrid_check_'.$gpmid.'\','.$i.','.$obriga.',2);"');
											echo "<input type=\"hidden\" name=\"montante_alterado_".$gpmid."[".$a."]\" value=\"0\">";
										} else {
											echo campo_texto( $var,$mtrmatriz,$habil,'',20,18,'','','right','',0,'onBlur="MouseBlur(this);" onkeyup="this.value=formata_decimal2(this.value);SomaValores('.$a.',\'mtmvlrlimite'.$gpmid.'_\',\'conta'.$gpmid.'\',1,\'mtrid_check_'.$gpmid.'\','.$i.','.$obriga.',2);"');
										}
									?>
								</td>
						    	<?php if ($idproposta != '') : ?>
							    	<td>
							    		<?=campo_texto($var2,'N','N','',20,18,'','','right','',0,'');?></td>
									</td>
								<? endif; ?>
			    		</tr>
				    	<?
						}
						  	$var = "mtmvlrlimite".$gpmid."_[$a]";
							${$var} = formata_valor( floatval( $mostraSub ), 0 );
					    	$var2 = "vlDistribuido".$gpmid."_[$a]";
							${$var2} = formata_valor( floatval( $mostraSubDistribuido ), 0 );
						?>
						<tr>
	    					<td align="right">
	    						<input type="hidden" name="conta<?=($gpmid)?>" value="<?=$conta;?>">
	    						Total:
	    					</td>
	    					<td>
	    						<?= campo_texto( "mtmvlrlimite".$gpmid."_[$a]",'N','N','',20,18,'','','right','',0,'onkeydown="this.value=formata_decimal2(this.value);"');?>
	    					</td>
					     	<? if ($idproposta != '') : ?>
					    	   	<td><?=campo_texto("vlDistribuido".$gpmid."_[$a]",'N','N','',20,18,'','','right','',0,'');?></td>
							<? endif; ?>
					    </tr> 
					<?
					}
					?>
					<input type="hidden" name="subTotal[<?=$i?>]" value="<?=${$var}?>">
					<?
				}
	    		$var = "mtmvlrlimiteTOTAL";
				${$var} = formata_valor(floatval($mostraTotal),0);
	    		$var2 = "TOTALDistribuido";
				${$var2} = formata_valor(floatval($mostraTotalDistribuido),0);
			?>
			<input type="hidden" name="contador2" value="<?=$z;?>">
			<script>
				var totalGrupos = <?=$i?>;
			</script>
			<tr>
		    	<td align="right">Total Geral:</td>
		    	<td><?=campo_texto("mtmvlrlimiteTOTAL",'N','N','',20,18,'','','right');?>
		    	</td>
		     	<?
		    	if ($idproposta != '') {
		    	?>
		    	    	<td><?=campo_texto("TOTALDistribuido",'N','N','',20,18,'','','right');?>
				<? } ?>  
		    	</td>
		    </tr> 
		<?
		}
		?>
		<?php endif; ?>
		<tr bgcolor="#C0C0C0">
			<?php
			
			if ( !$_REQUEST['idproposta'] )
			{
				?>
				    <td colspan="3" class="SubTituloDireita" align="left">
				    <input type='button' class="botao" align="left" value='Salvar' onclick="gravar_janela('I')"></td>
			    <?
			}
			else
			{
				?>
				    <td class="SubTituloEsquerda" >
					    <input type='button' class="botao" value='Adicionar Coluna' onclick="InsereColunas(<?=$idproposta?>,<?=$ppoano?>)">
					    &nbsp;&nbsp;
					    <? if ( $PropostaAtivaAtual == 0 ) : ?>
					    	<input type='button' class="botao" value='Ativar Proposta' onclick="AtivarProposta(<?=$idproposta?>)">
					    <? endif; ?>
					    &nbsp;&nbsp;
					    <input type='button' class="botao" value='Copiar Proposta' onclick="GerarCopia(<?=$idproposta?>)">
				    </td>
					<td colspan="2" class="SubTituloDireita" valign="center">     
				    	<input type='button' class="botao" value='Salvar' onclick="gravar_janela('A')">
				    </td>
				<?
			}
			?>
		</tr>
	</table>
	
</center>
</div>
<br/><br/>
</form>

<script type="text/javascript">

var idSubTotais = new Array();
var chaveidSubTotais = new Array();

function SomaValoresSub(){
	var resultado = 0;
	var vl = 0;

	for (i=0; i < totalGrupos; i++)
	{
		vl = document.forms['formulario'].elements['subTotal['+i+']'].value;
//		alert(vl);
		vl = vl.replace(/\./g,'')
		vl = vl.replace(',','.')
		if (vl) resultado += parseFloat(vl);
//		alert(resultado);
	}
//	alert(resultado);
	if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] )
		document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value = formata_decimal ( resultado );
	
	
}


function SomaValores(cod, prefixo, contador, geral, campoCheck, idSubTotal, obrigatorio, grupo){
	var vlTotal = 0, vl = 0, retorno = 0, vlgeral = 0, vllinha = 0;
	var a=0;
	var str = new String;
	
	a = document.forms['formulario'].elements[contador].value;
	vlgeral = '';
	if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] )
		vlgeral = document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value;
	if (vlgeral == '')	vlgeral = 0;

//alert(document.forms['formulario'].elements[prefixo+'['+i+']'].value);
	for (var i = 0; i<=(a-1); i++){

		if (document.forms['formulario'].elements[prefixo+'['+i+']'].value != '')
		{	
			<? if ($idproposta == '') { ?>
			if (campoCheck != '' && cod == i)	
			{
				if (document.forms['formulario'].elements[campoCheck+'['+i+']'].checked == false)
					document.forms['formulario'].elements[campoCheck+'['+i+']'].checked = true;
			}
								<? } ?>
			vlTotal = document.forms['formulario'].elements[prefixo+'['+i+']'].value;

				if (vlTotal != '')
			{
				vlTotal = vlTotal.replace(/\./g,'')
				vlTotal = vlTotal.replace(',','.')
			}
			else
			{
				vlTotal = 0
			}
			

			retorno = parseFloat ( retorno ) + parseFloat( vlTotal );
	
			document.forms['formulario'].elements[prefixo+'['+a+']'].value =  formata_decimal ( retorno );
			if (idSubTotal != null)
			{	
				document.forms['formulario'].elements['subTotal['+idSubTotal+']'].value = formata_decimal ( retorno );
				SomaValoresSub();
			}
			if (obrigatorio != null && grupo == 1 && cod == i)
			{	
				vllinha = parseFloat ( vllinha ) + parseFloat( vlTotal );
				document.forms['formulario'].elements['obriga1['+obrigatorio+']'].value = formata_decimal ( vllinha );
			}
			if (obrigatorio != null && grupo == 2 && cod == i)
			{	
				vllinha = parseFloat ( vllinha ) + parseFloat( vlTotal );
				document.forms['formulario'].elements['obriga2['+obrigatorio+']'].value = formata_decimal ( vllinha );
			}

		}

	
	}


}
<? if ($idproposta == '') { ?>
function InseriuValor(campo,i,id,contador){
	
		var valor = 0, a =0, resultado = 0, vlGrupo = 0;
		
		a = document.forms['formulario'].elements[contador].value;
		vlGrupo = document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+a+']'].value;
		valor = document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+i+']'].value;
		vlTotal = 0;
		if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] ) 
			vlTotal = document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value;
		if (document.forms['formulario'].elements[campo+'['+i+']'].checked == false)
		{
			if (valor != '') document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+i+']'].value = '';
			if (valor != '')
			{
				valor = valor.replace(/\./g,'');
				valor = valor.replace(',','.');
				valor = parseFloat(valor);
				vlTotal = vlTotal.replace(/\./g,'');
				vlTotal = vlTotal.replace(',','.');
				vlTotal = parseFloat(vlTotal);
				vlGrupo = vlGrupo.replace(/\./g,'');
				vlGrupo = vlGrupo.replace(',','.');
				vlGrupo = parseFloat(vlGrupo);
				resultado = parseFloat(vlTotal)- parseFloat(valor);
				valor = parseFloat(vlGrupo)- parseFloat(valor);
				if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] ) 
					document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value = formata_decimal ( resultado );

				document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+a+']'].value = formata_decimal ( valor );
				document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+i+']'].value = formata_decimal ( 0 );

			}
			else
				valor = parseFloat(0);
				vlTotal = vlTotal.replace(/\./g,'');
				vlTotal = vlTotal.replace(',','.');
				vlTotal = parseFloat(vlTotal);
				vlGrupo = vlGrupo.replace(/\./g,'');
				vlGrupo = vlGrupo.replace(',','.');
				vlGrupo = parseFloat(vlGrupo);
				resultado = parseFloat(vlTotal)- parseFloat(valor);
				valor = parseFloat(vlGrupo)- parseFloat(valor);
				if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] ) 
					document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value = formata_decimal ( resultado );
				
				document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+a+']'].value = formata_decimal ( valor );
				document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+i+']'].value = formata_decimal ( 0 );
			
		}
		
/*		else
		{
			if (valor == '0,00') 
			{
				alert('É necessário inserir o valor!');
				document.forms['formulario'].elements['mtmvlrlimite'+id+'_['+i+']'].focus();	
			}
	
		}	
*/
}
<? } ?>


function formata_decimal( valor )
{

	valor = parseFloat(valor);
//	valor = valor.toFixed(2);
	valor += '';
	valor = valor.replace(',',''); 
	valor = valor.replace('.',''); 


	return mascaraglobal( '###.###.###.###', valor );

}

function formata_decimal2( valor )
{

		valor += '';
	// retira os caracteres que não são numericos
	for ( var i = 0 ; i < valor.length; )
	{
		if ( isNaN( valor.charAt( i ) ) == true )
		{
			valor = valor.replace( valor.charAt( i ), '' );
			continue;
		}
		i++;
	}
	// retira zeros a esquerda
	while ( valor.charAt( 0 ) == '0' )
	{
		valor = valor.substr( 1 );
	}
	// adiciona zeros a esquerda caso haja necessidade
/*	switch ( valor.length )
	{
		case 0:
			valor = '000' + valor;
			break;
		case 1:
			valor = '00' + valor;
			break;
		case 2:
			valor = '0' + valor;
			break;
	}
*/
	return mascaraglobal( '###.###.###.###', valor );

}


function InsereColunas(cod, ano)
{
	window.open("elabrev.php?modulo=principal/propostaorcamentaria/configuracao/insereColunaMontante&acao=A&ppoid="+cod+"&ano="+ano,'formulario','left=300,width=600,height=450,status=0,toolbar=0,location=0,scrollbars=1,resizable=1');
}

function InsereComentarios(cod)
{
	window.open("elabrev.php?modulo=principal/propostaorcamentaria/configuracao/comentarios&acao=I&ppoid="+cod,'formulario','left=300,width=600,height=450,status=0,toolbar=0,location=0,scrollbars=1,resizable=1');
}

function gravar_janela(cod)
   {
   	var a = 0, vlCategoria = 0, vlColuna = 0, grupo = 0, indice = 0;
	
	<?php if ( $idproposta ) : ?>
	
	   	vlCategoria = document.forms['formulario'].elements['lmpvalor[<?=$categUltimo?>]'].value;
	   	if ( document.forms['formulario'].elements['mtmvlrlimiteTOTAL'] ) 
	   		vlColuna = document.forms['formulario'].elements['mtmvlrlimiteTOTAL'].value;
	   	else
	   		vlColuna = '';
		
	   	vlCategoria = vlCategoria.replace(/\./g,'');
		vlCategoria = vlCategoria.replace(',','.');
		vlCategoria = parseFloat(vlCategoria);
	   	vlColuna = vlColuna.replace(/\./g,'');
		vlColuna = vlColuna.replace(',','.');
		vlColuna = parseFloat(vlColuna);
	
		
		if ( vlCategoria < vlColuna )
		{
			alert ('O total geral das colunas não pode ser maior \n que o total por Categoria Econômica');
			return;
		}	
	
	<?php endif; ?>
	
		a = document.forms['formulario'].elements['contador1'].value;
		for (var i = 0; i<=(a-1); i++)
		{
			if ( document.forms['formulario'].elements['lmpvalor['+i+']'].value == '' )
			{
				alert('É necesário preencher o valor para as Matrizes Obrigatórias');
				document.forms['formulario'].elements['lmpvalor['+i+']'].focus();
				return;
			}
			
		}
	
	<?php if ( $idproposta ) : ?>
	    a = document.forms['formulario'].elements['contador2'].value;
		for (var i = 0; i<=(a-1); i++)
		{
			
			grupo = document.forms['formulario'].elements['grupo['+i+']'].value;
			indice = document.forms['formulario'].elements['indice['+i+']'].value;
			<?// if ( $_REQUEST['idproposta'] ) :?>
				if ( document.forms['formulario'].elements['mtmvlrlimite'+grupo+'_['+indice+']'].value == '' )
			<?// else : ?>
	//				if (document.forms['formulario'].elements['obriga2['+i+']'].value )
			<?// endif; ?>
			{
				alert('É necesário preencher o valor para as Matrizes Obrigatórias');
				document.forms['formulario'].elements['mtmvlrlimite'+grupo+'_['+indice+']'].focus();
				return;
			}
			
		}
	<?php endif; ?>
  
   	if (!validaBranco(document.formulario.ppodsc, 'Descrição')) return;	 
		  	
	if (!validaBranco(document.formulario.ppoano, 'Ano')) return;	 

	if (cod=='I') document.formulario.acao_n.value='inclui';
   	   	else document.formulario.acao_n.value='altera';
   	   	// checar consistencias

   	   	
   	   	document.formulario.submit();
   }
   
function ExcluirColuna(cod, descricao, mtmid)
{

	if (confirm('Tem certeza que deseja excluir a coluna ' + descricao + ' !'))
	{
	   	document.formulario.acao_n.value='excluirColuna';
	   	document.formulario.codColuna.value=cod;
	   	document.formulario.codMatriz.value=mtmid;
   	   	document.formulario.submit();
	}
	else
	return;   	   	
}

function AtivarProposta(cod)
{
<? 
	$qtdDespesa=$db->pegaUm( "select count( d.iducod ) from elabrev.despesaacao d inner join elabrev.ppaacao_orcamento ppa ON ppa.acaid = d.acaid where ppa.prgano = '".$_SESSION['exercicio']."'");
	if ($qtdDespesa != 0){?>
		if (confirm('Existem despesas lançadas para o exercício de <?=$_SESSION['exercicio']?>!'))	
			return;
<?	} ?>
	
<?	if ($qtdAtiva != 0){?>
		if (confirm('A proposta <?=$NomeAtiva?> já está ativa! \n Deseja ativar a <?=$ppodsc?>?'))	
		{
			document.formulario.acao_n.value='AtivarProposta';
		   	document.formulario.idproposta.value=cod;
	   	   	document.formulario.submit();
		}
<? }
	else 
	{
?>
			document.formulario.acao_n.value='AtivarProposta';
		   	document.formulario.idproposta.value=cod;
	   	   	document.formulario.submit();

<?	}?>
}

function GerarCopia(cod)
{
	   	document.formulario.acao_n.value='GerarCopia';
	   	document.formulario.idproposta.value=cod;
   	   	document.formulario.submit();
}

function AbrirFracao(cod,valor, dsc, campo)
{
	if (valor == '') 
	{
		alert('Não existe valor cadastrado para a Coluna '+dsc+' !');
		document.forms['formulario'].elements[campo].focus();
		return;
	}
	else
	location.href='elabrev.php?modulo=principal/propostaorcamentaria/fracao&acao=I&mtmid='+cod;
}

function AbrirLimite(cod,valor, dsc, campo)
{
	location.href='elabrev.php?modulo=principal/propostaorcamentaria/limitefonte&acao=A&mtmid='+cod;
}
   
function marcarMontanteAlterado(name) {
   var campo = document.getElementsByName(name)[0];
   
   campo.value = "1";
}
 
 /**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var td = document.getElementsByName( bloco + '_td[]' );
	var img = document.getElementById( bloco + '_img' );
	
	if ( td[0].style.display == 'none' )
	{
		for(var i=0; i<td.length; i++) { 
			td[i].style.display = 'table-row';
		}
		img.src = '/imagens/menos.gif';
	}
	else
	{
		for(var i=0; i<td.length; i++) { 
			td[i].style.display = 'none';
		}
		img.src = '/imagens/mais.gif';
	}
}
	
</script>