<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']();
    }
    
    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Sessão expirou. Tente novamente!");
    }
    
    verificaMantenedora();
    
    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Cadastro de Hospital' );

  

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/tramite&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    

?>
<html>
 <head>
  <script type="text/javascript" src="/includes/prototype.js"></script>
  <script type="text/javascript" src="../includes/funcoes.js"></script>
  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

 
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">

<?php
$perfil = pegaPerfilGeral();
$mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = " . $mntid;
$dados_mantenedora = $db->carregar($sql);


$sql1 = "
        SELECT  count(m.mtdid) as quantidade
        FROM maismedicomec.mantidacandidata AS m

        JOIN maismedicomec.mantidamunicipiocandidata AS mm ON mm.mtdid = m.mtdid

          JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
         JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
         JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}

        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}";

		if(in_array(PERFIL_HOSPITAL, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_HOSPITAL;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
        if(in_array(PERFIL_IES, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_IES;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
    

  $RS = $db->recuperar($sql1,$res);
   $RS['quantidade'];


?>
<table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

    <tr>
        <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
        <td><?php echo $mntid ?></td>
    </tr>

    <tr>
        <td class ="SubTituloDireita" width="35%">Razão social</td>
        <td><?php echo $dados_mantenedora[0][mntrazaosocial]; ?></td>
    </tr>
</table>

<form id="formulario" name="formulario" method="post"  >

	<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td bgcolor="#e9e9e9" align="center">
				<b>Pendências</b>
			</td>
		</tr>
		<tr>
			<td align="left">
					
					<table width="30%" align="center">
						<tr>
							<td nowrap="nowrap">
							
								<br>
								
								<?
								//verifica matida indicada
								/*
								$sql = "select count(mntid) from maismedicomec.mantidamunicipio where mntid = " . $mntid;
								$total = $db->pegaUm($sql);
								if($total == 0) echo '<font color="red"><b> - Aba Mantida indicada: Favor cadastrar uma instituição.</b></font><br>'; 
								*/
								
								//verifica Corpo dirigente da Mantida
								$sql = "select count(c.cpdid) from maismedicomec.corpodirigente AS c
       									inner join maismedicomec.dirigentemantida AS d ON d.cpdid = c.cpdid
										left join maismedicomec.mantidamunicipio m on m.mtdid = d.mtdid
										left join maismedicomec.mantidamunicipiocandidata m2 on m2.mtdid = d.mtdid
										where c.cpdstatus='A' and (m.mntid =  " . $mntid." or m2.mntid =  " . $mntid.")";
								$total = $db->pegaUm($sql);
								if($total == 0) echo '<font color="red"><b> - Aba Corpo dirigente da Mantida: Favor cadastrar um dirigente.</b></font><br>';
								
								//verifica Corpo dirigente da Mantida
								/*
								$sql = "select count(mntid) from maismedicomec.mantidamunicipiocandidata where mntid = " . $mntid;
								$total = $db->pegaUm($sql);
								if($total == 0) echo '<font color="red"><b> - Aba Credenciar Nova mantida: Favor cadastrar uma instituição.</b></font><br>';
								*/
								
								//verifica Experiência Regulatória
								/*
								$sql = "select count(mntid) from maismedicomec.mantenedoraexpregulatoria where mntid = " . $mntid;
								$total = $db->pegaUm($sql);
								if($total == 0) echo '<font color="red"><b> - Aba Experiência Regulatória: Favor cadastrar as instituições, cursos e programas.</b></font><br>';
								*/
								
								//verifica matida indicada = portifolio matida indicada
								//$sql = "select count(mntid) from maismedicomec.mantidamunicipio where mntid = " . $mntid;
								$sql = "select count(mtdid) from maismedicomec.mantida where mtdid in (select mtdid from maismedicomec.mantidamunicipio where mntid = {$mntid} group by mtdid)";
								$total = $db->pegaUm($sql);
								
								$sql = "select count(mntid) from maismedicomec.mantidaexpregulatoria where mnestatus='A' and mntid = " . $mntid;
								$total2 = $db->pegaUm($sql);
								if($total != $total2) echo '<font color="red"><b> - Aba Portfólio de Mantidas: para cada Mantida Indicada inscrita, é obrigatório o preenchimento da mesma nas Abas Portfólio de Mantidas/Mantida Indicada.</b></font><br>'; 
								
								?>
								
			
								<br>
								
								<center>
								<?php
									if($_SESSION['maismedicomec']['mntid']){
										$docid = criaDocidMantenedora( $_SESSION['maismedicomec']['mntid'] ); 
										
										if($docid){
											wf_desenhaBarraNavegacao( $docid , array("mntid" => $_SESSION['maismedicomec']['mntid']) );
										} 
									}
								?>
								</center>
				
							</td>
						</tr>
					</table>
			</td>
		</tr>
	</table>
	
</form>

</body>

</html>