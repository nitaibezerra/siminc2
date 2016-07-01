<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];
$andDataLimite = " and a.avldtavaliacao > '2013-12-31'";
if(!$entid){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?php
	exit;
}

// ver($_SESSION,d);
//carrega dados para edição
if($_REQUEST['nutid']){
	$sql = "SELECT  
				  nutid,
				  nutprimciclo,
				  nutsegunciclo
		     FROM 
		     		pse.nutrisus
		     WHERE 
		     		nutid = {$_REQUEST['nutid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}


//recupera o idebp
$dadosEscola = $db->pegaLinha("select esc.idebp,ede.muncod, ede.estuf
							   from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
							   left join entidade.endereco ede on ede.entid = ent.entid
							   where ent.entid = {$entid}");
if($dadosEscola) extract($dadosEscola);

if(!$idebp){
	?>
	<script>
		alert("Essa Escola não possui acesso .");
		//location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?php
	//exit;
}



//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
//if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';



//excluir registro
if($_REQUEST['exluir'] && $_REQUEST['nutid']){
    
    //inserir equipe ESF
	$sql = "DELETE FROM pse.scnesnutrisus WHERE nutid = ".$nutid;
	$db->executar($sql);
	
	
	$sql = "DELETE FROM pse.nutrisus WHERE nutid = ".$_REQUEST['nutid'];
	$db->executar($sql);
	
        $db->commit();
	
	print "<script>
		alert('Operação realizada com sucesso!');
		location.href='pse.php?modulo=principal/nutrisus&acao=A';
	   </script>";
	
	exit();
}


//submit
if($_POST){

	if(!$_POST['idebp']){
		?>
		<script>
			alert("Sua escola não está na base do PSE. Favor entrar em contato com o Gestor do sistema.");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?php
		exit;	
	}
	//verifica o somatorio do Número de educandos pactuados (avleducandoavaliado)

		
	$nutid = $_REQUEST['nutid']; 
	
	if(!$nutid) { //INSERIR
		
		$sql = "INSERT INTO pse.nutrisus
			(
                            muncod, 
                            idebp, 
                            iduf, 
                            nutprimciclo, 
                            nutsegunciclo, 
                            nutdatregistro, 
                            usucpf
                        ) VALUES (
	        	'".$_REQUEST['muncod']."',
	        	".$_REQUEST['idebp'].",
                        '".$_REQUEST['estuf']."',
	        	'".$_REQUEST['primeiroCiclo']."',
                        '".$_REQUEST['segundoCiclo']."',
	        	NOW(),
                        '".$_SESSION['usucpf']."'
	        ) returning nutid";	
            
		$nutid = $db->pegaUm($sql);
                
//                $sqlscnes = "INSERT INTO pse.scnesnutrisus (nutid, cneid) VALUES ('".$nutid."', )  ";
//                $db->executar($sqlscnes);
                
                
		
	}
	else{ //ALTERAR
		
		$sql = "UPDATE 
					 pse.nutrisus
				SET 
					nutprimciclo = ".$_REQUEST['primeiroCiclo'].",
                                        nutsegunciclo = ".$_REQUEST['segundoCiclo']."
				WHERE 
					nutid = ".$nutid;
		$db->executar($sql);
	
	}
        
        //inserir equipe ESF
	$sql = "DELETE FROM pse.scnesnutrisus WHERE nutid = ".$nutid;
	$db->executar($sql);
	
	if($_REQUEST['cneid'][0]){
		foreach($_REQUEST['cneid'] as $v){
			$sql = "INSERT INTO pse.scnesnutrisus (nutid, cneid) 
					VALUES (".$nutid.",".$v.")";
			$db->executar($sql);
		}
	}
	
		
	$db->commit();
	
	unset($_REQUEST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/nutrisus&acao=A';
		   </script>";
	exit();
	
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

	$titulo = "Nutrisus";

monta_titulo( $titulo, '' );

Cabecalho($entid,1);
?>
<script>

function salvar(){
    var d = document.formulario;

    if("" == d.primeiroCiclo.value){
       alert("Primeiro ciclo tem preenchimento obrigatório.");
        return false;
    }
     if("" == d.segundoCiclo.value){
        alert("Segundo ciclo tem preenchimento obrigatório.");
        return false;
    }
    selectAllOptions( document.getElementById( 'cneid' ) );
    d.submit();
    
}

function alterarNutrisus(id){
	location.href='pse.php?modulo=principal/nutrisus&acao=A&nutid='+id;
}

function excluirNutrisus(id){

	if(confirm('Deseja realmente excluir este item?')){
		location.href='pse.php?modulo=principal/nutrisus&acao=A&exluir=1&nutid='+id;
	}
}
</script>


<br>
<form id="formulario" name="formulario" action="" method="post">
    
<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;
                    <input type="hidden" name="idebp" id="idebp" value="<?php echo $idebp; ?>">
                    <input type="hidden" name="muncod" id="muncod" value="<?php echo $muncod; ?>">
                    <input type="hidden" name="estuf" id="estuf" value="<?php echo $estuf; ?>">
                    <input type="hidden" name="nutid" id="nutid" value="<?php echo $nutid; ?>">
                </td>
	</tr>
        <tr>
		<td width="40%" class="SubTituloDireita" colspan='2'>Informe a Equipe da Atenção Básica, caso esta tenha participado da ação:</td>
		<td colspan='2'>
			<?php
				/*
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$_SESSION['pse']['muncod']."'
							  --and cnetppopassistescola='A'
							  order by 2";
				*/
			
				$sql_combo = "SELECT
								cneid as codigo,
								cnecodigocnes ||' - '|| cnenomefantasia ||' - '|| cneseqequipe as descricao
							  FROM pse.scnes
							  where muncod = '".$muncod."'
							  order by 2";

				if ($nutid){					
					$sql = "SELECT
								a.cneid as codigo,
								a.cnecodigocnes ||' - '|| a.cnenomefantasia ||' - '|| a.cneseqequipe as descricao
							  FROM pse.scnes a
							  inner join pse.scnesnutrisus s on s.cneid = a.cneid
							  where nutid = ".$nutid." 
							  --and cnetppopassistescola='A'
							  order by 2";
					//dbg($sql);
					//$nome = 'cneid';
					//$$nome = $db->carregar( $sql ); 
					$cneid = $db->carregar( $sql );
				}
                                	combo_popup( 'cneid', $sql_combo, 'Selecione a(s) Equipes(s)', '360x460', '', '', '', $vPermissao, true, '', '5', '400', '', '', false, $where = '', '', $mostraPesquisa = true, $campo_busca_descricao = false, '', false, '' , '' );
				//echo obrigatorio();
			?>
				
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Número de crianças que concluíram o primeiro ciclo (mínimo de 36 sachês)</td>
		<td colspan='2'>
			<?=campo_texto( 'primeiroCiclo', 'S', $vPermissao, '', 9, 6, '######', '', 'right', '', 0, '','',$nutprimciclo);?>
                    <b> 1º. Semestre/2015</b></td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Número de crianças que concluíram o segundo ciclo (mínimo de 36 sachês)</td>
		<td colspan='2'>
			<?=campo_texto( 'segundoCiclo', 'S', $vPermissao, '', 9, 6, '######', '', 'right', '', 0, '','',$nutsegunciclo);?>
                    <b> 2º. Semestre/2015</b></td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="salvar();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/listaNutrisus&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	<tr>
		<td colspan='4'>
			<div id="listaC1" style="height: 150px; overflow-y: auto; overflow-x: hidden;">
				<?php 
					if(!$idebp) $idebp = 0;
                                        if ($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA) {
                                                $sql = "SELECT
									'<center>' ||
									(CASE WHEN '{$vPermissao}' = 'S' THEN
											 '<a href=\"#\" onclick=\"alterarNutrisus(\'' ||nut.nutid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\" onclick=\"excluirNutrisus(\'' || nut.nutid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
										  ELSE
											 '<a href=\"#\"><img src=\"../imagens/alterar_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>&nbsp;<a href=\"#\"><img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
									END) || '</center>'  as acao,
					            	iduf, mundsc,  
					            	'<center>'||to_char(nutdatregistro::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
                                                        nutprimciclo, nutsegunciclo   
                                                        , array_agg(cnecodigocnes||' - '||cnenomefantasia||' - '||cneseqequipe) as nome
  FROM pse.nutrisus nut 
    inner join municipio mun on nut.muncod = mun.muncod
    left join pse.scnesnutrisus scn on nut.nutid = scn.nutid
    left join pse.scnes sce on scn.cneid = sce.cneid
  where idebp = '{$idebp}' group by nut.nutid, iduf, mundsc, nutdatregistro,  nutprimciclo, nutsegunciclo";

    $cabecalho = array("Ação","UF", "Município", "Data do Registro","Primeiro Ciclo", "Segundo Ciclo", "Equipes");
    $alinha = array("left", "center", "center", "center", "center");
    $tamanho = array("20%", "20%", "20%", "20%", "20%");
} else {
    $sql = "SELECT distinct iduf, mundsc,					            	
        '<center>'||to_char(nutdatregistro::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
 nutprimciclo, nutsegunciclo
 , array_agg(cnecodigocnes||' - '||cnenomefantasia||' - '||cneseqequipe) as nome
  FROM pse.nutrisus nut 
    inner join municipio mun on nut.muncod = mun.muncod
    left join pse.scnesnutrisus scn on nut.nutid = scn.nutid
    left join pse.scnes sce on scn.cneid = sce.cneid
  where idebp =  '{$idebp}' group by iduf, mundsc, nutdatregistro,  nutprimciclo, nutsegunciclo";

    $cabecalho = array("UF", "Município", "Data do Registro","Primeiro Ciclo", "Segundo Ciclo", "Equipes");
    $alinha = array("left", "center", "center", "center", "center");
    $tamanho = array("20%", "20%", "20%", "20%");
}

$dadosListagem = $db->carregar($sql);
$dadosListagem = $dadosListagem ? $dadosListagem : array();
//
foreach( $dadosListagem as &$valores ){
    $valores[nome] = str_replace('{"',"",$valores[nome]);
    $valores[nome] = str_replace('"}',"",$valores[nome]);
    $valores[nome] = str_replace('","'," / ",$valores[nome]);
}
   
//	
$db->monta_lista_simples( $dadosListagem, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
				?>
				
				
			<div>
		</td>
	</tr>
</table>
</form>
