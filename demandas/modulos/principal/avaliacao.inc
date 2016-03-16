<?php
if(!$_SESSION['dmdid']){
	unset($_SESSION);
	print "<script>
				alert('Sua sessão expirou. Acesse novamente o link para avaliar a demanda!');
				history.back(); 
		   </script>";
	exit;
}

$dmdid = $_SESSION['dmdid'];

//filtra o andar
if ($_POST['avdidAjax']) {
	inserirResp($_POST['avdidAjax']);
	exit;
}

function inserirResp($avdid){
	global $db;
	
	$sql = "UPDATE 
				demandas.avaliacaodemanda SET 
				avdresposta			  = '".utf8_decode($_POST['resposta'])."',
				usucpftecnico		  = '".$_SESSION['usucpf']."',
				avddtresposta		  = '".date('Y-m-d H:i:s')."' 
			WHERE 
				avdid 				  = $avdid
			";
	$db->executar($sql);
	$db->commit();
	
	echo "OK";
	exit;
}

$perfilUserAv	= arrayPerfil();

if ( !in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUserAv) 
        && !in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfilUserAv) 
        && !in_array(DEMANDA_PERFIL_GESTOR_MEC,$perfilUserAv) 
        && !in_array(DEMANDA_PERFIL_DBA,$perfilUserAv) 
        && !in_array(DEMANDA_PERFIL_GESTOR_REDES,$perfilUserAv) ){
    
	if( in_array(DEMANDA_PERFIL_TECNICO, $perfilUserAv ) || in_array(DEMANDA_PERFIL_TECNICO1, $perfilUserAv ) ){
		print "<script>
					alert('Você não possui permissão para acessar esta página!');
					history.back(); 
			   </script>";
		exit;
	}
}

$habil = 'N';
$mostraWorkflow = 'N';

//(ver)ifica se o usuário pode editar a demanda
$verificaEditaDemanda = verificaEditaDemanda();
if($verificaEditaDemanda == true){
	$habil = 'S';
	$mostraWorkflow = 'S';
}
else{
	$habil = 'N';
	$mostraWorkflow = 'N';
}

// Pega o "estado do documento", vinculado à demanda
$esdid = recuperaEstadoDocumento();

//verifica se a avaliação é do demandante mesmo.
$sql = "select count(dmdid) as total from demandas.demanda where dmdid=".$dmdid." AND usucpfdemandante = '" . $_SESSION['usucpf'] . "'"; 
$total = $db->PegaUm($sql);
if ( $total > 0 ){
	$habil = 'S';
	$mostraWorkflow = 'S';
}

//verifica se o estado do documento é finalizada ou cancelada (se sim, bloqueia todos os campos).
//if( !$esdid || in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_FINALIZADO,DEMANDA_GENERICO_ESTADO_CANCELADO)) ){
if( !$esdid || in_array($esdid, array(DEMANDA_ESTADO_CANCELADO,DEMANDA_GENERICO_ESTADO_CANCELADO)) ){
	$habil = 'N';
}

function inserirAv(){
	global $db;
	$sql = " INSERT INTO demandas.avaliacaodemanda
			 (
			 	dmdid, avdprobres, avdtempo, avdtecnico, avdgeral, avsobs, avdstatus, avddata
			 ) VALUES (
			 	".$_SESSION['dmdid'].", 
			 	'".$_POST['avdprobres']."',
			 	'".$_POST['avdtempo']."',
			 	'".$_POST['avdtecnico']."',
			 	'".$_POST['avdgeral']."',
			 	'".$_POST['avsobs']."',
			 	'A',
			 	'".date('Y-m-d H:i:s')."'
			 );";
        
         $sqlLimpa = "UPDATE demandas.avaliacaodemanda
                        SET    avnegatividade = ''
                        WHERE  dmdid = '{$_SESSION['dmdid']}' ";
        
        if(($_POST['avdtecnico'] == "3" or $_POST['avdtecnico'] == "4") && ($_POST['avdgeral'] == "3" or $_POST['avdgeral'] == "4")){
            $db->executar($sqlLimpa);
        }                
        
	$db->executar($sql);
	$db->commit();
}


if ($_POST['UpdateNegatividade']){
    $dmdid = $_SESSION['dmdid'];
    
        if($_POST['UpdateNegatividade'] == "trueChecked"){
            $postNegatividade = "1";
        }else if(($_POST['UpdateNegatividade'] == "falseChecked") or ($_POST['UpdateNegatividade'] == "")){
            $postNegatividade = "";     
        }
    
        global $db;
        $sqlLimpa = "UPDATE demandas.avaliacaodemanda
                        SET    avnegatividade = ''
                        WHERE  dmdid = '{$dmdid}' ";
    
        
	$sql = "UPDATE demandas.avaliacaodemanda
                SET    avnegatividade = '{$postNegatividade}'
                WHERE  avddata = (SELECT Max(avddata) AS data
                                FROM   demandas.avaliacaodemanda
                                WHERE  dmdid = '{$dmdid}')
                    AND dmdid = '{$dmdid}' ";
                    
	$db->executar($sqlLimpa);
        $db->executar($sql);
	$db->commit();
        echo "OK";
    exit;
}

if($_POST){
	
	$sql = "select dmddatainclusao 
			from demandas.demanda 
			where dmdid = ".$_SESSION['dmdid']."
			and (dmddatainclusao + INTERVAL '30 DAYS') > now()";
	$dmddatainclusao = $db->pegaUm($sql);
	
	if(!$dmddatainclusao){
		print "<script> alert('Não é Possível avaliar, pois já se passaram mais de 30 dias do atendimento desta demanda!'); </script>";
	}
	else{
            
                    inserirAv();
                    //envia email para os gestores
                    if($_POST['avdgeral'] == '1' || $_POST['avdgeral'] == '2'){

                            if($_POST['avdgeral'] == '1') $flag = "RUIM";
                            if($_POST['avdgeral'] == '2') $flag = "REGULAR";

                            enviaEmailAvaliacaoRuim($_SESSION['dmdid'], $flag, $_POST['avsobs']);

                }
		
		print "<script> alert('Operação realizada com sucesso!'); </script>";
		
	}
	unset($_POST);
	print "<script> location.href='demandas.php?modulo=principal/avaliacao&acao=A'; </script>";
	exit();
	
}


include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";

/*
 * Recupera o "docid"
 * Caso não exista "docid" cadastrado na demanda 
 * Insere o documento e vincula na demanda
 */
$docid = criarDocumento( $dmdid );

print '<br>';
$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Avaliação da Demanda - Cód. # '.$dmdid, '' );
?>
<html>
 <head>
     <style type="text/css">
        .checkbox {
            position: relative;
            top: 3px;
        }
     </style>
     
     

  <script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

  <script type="text/javascript">
	function validaForm(){
	 	d = document;

		if(!d.formA.avdprobres[0].checked && !d.formA.avdprobres[1].checked){
			alert ('É necessário responder a questão de nº 1!');
			return false;
		}
		if(!d.formA.avdtempo[0].checked && !d.formA.avdtempo[1].checked && !d.formA.avdtempo[2].checked && !d.formA.avdtempo[3].checked){
			alert ('É necessário responder a questão de nº 2!');
			return false;
		}
		if(!d.formA.Truim.checked && !d.formA.Tregular.checked && !d.formA.Tbom.checked && !d.formA.Totimo.checked){
			alert ('É necessário responder a questão de nº 3!');
			return false;
		}
		if(!d.formA.Gruim.checked && !d.formA.Gregular.checked && !d.formA.Gbom.checked && !d.formA.Gotimo.checked){
			alert ('É necessário responder a questão de nº 4!');
			return false;
		}
		if((d.formA.Gruim.checked || d.formA.Gregular.checked) && d.formA.avsobs.value==''){
			alert ('É necessário justificar a questão de nº 4 no campo Observação');
			return false;
		}
		
		return true;
	}	

	function exibeHistoricoDem( docid )
	{
		var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
			'?modulo=principal/tramitacao' +
			'&acao=C' +
			'&docid=' + docid;
		window.open(
			url,
			'alterarEstado',
			'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
		);
	}				
	
  </script>
 </head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?=cabecalhoDemanda(); ?>
<form id="formA" name="formA" action="" method="post" onsubmit="return validaForm();">

<input type="hidden" name="avdid" value="<?=$avdid?>">

<table border=0 class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita">Você esta avaliando o serviço do técnico:</td>
		<td>
                    <?php 
                    $tecnico_demanda = "-";
                    if (!empty($dmdid)) {
                            $sql = "
                                SELECT 
                                    usuario.usunome as tecnico_demanda
                                FROM  
                                    demandas.demanda demanda
                                    INNER JOIN seguranca.usuario usuario
                                    ON usuario.usucpf = demanda.usucpfexecutor
                                AND demanda.dmdid = '{$dmdid}'  
                                ";

                            $tecnico_demanda = $db->pegaUm($sql);
                    } 
                    echo $tecnico_demanda;
                    ?>
                </td>
	</tr>
        <tr>
                <td width="25%" class="subtitulodireita">1. Sua solicitação foi atendida e seu problema foi resolvido?</td>
		<td >
			<input type="radio" name="avdprobres" value="S" <?if($avdprobres == 'S') echo 'checked';?>> Sim
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdprobres" value="N" <?if($avdprobres == 'N') echo 'checked';?>> Não
		</td>
		<td rowspan="7" style="background-color:#fafafa; color:#404040; width: 1px;" valign="top" align="left">
			<?php 
				if($mostraWorkflow == 'S'){
					// Barra de estado atual e ações e Historico
					wf_desenhaBarraNavegacao( $docid , array( 'unicod' => '' ) );	
					//Barra outras opções
					criarNovaDemanda(); //www/demandas/_funcoes.php
				}else{
					?>
						<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>estado atual</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<?php 
												if($esdid){
													$sqlesd = "SELECT esddsc
															FROM workflow.estadodocumento
															WHERE esdid = $esdid";
													
													echo $db->pegaUm($sqlesd);
												}
												else{
													echo "Em Processamento";
												}
												?>					
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>ações</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												você não possui permissão
											</span>
										</td>
									</tr>
									<tr style="background-color: #c9c9c9; text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<span title="estado atual">
												<b>histórico</b>
											</span>
										</td>
									</tr>
									<tr style="text-align:center;">
										<td style="font-size:7pt; text-align:center;">
											<img style="cursor: pointer;" src="http://<?=$_SERVER['SERVER_NAME']?>/imagens/fluxodoc.gif"	 onclick="exibeHistoricoDem( '<?=$docid?>' );">
										</td>
									</tr>
									
						</table>					
					<? 
				}
			?>	
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">2. O tempo que demorou para você receber o atendimento foi:</td>
		<td >
			<input type="radio" name="avdtempo" value="M" <?if($avdtempo == 'M') echo 'checked';?>> Muito Bom
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="B" <?if($avdtempo == 'B') echo 'checked';?>> Bom
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="A" <?if($avdtempo == 'A') echo 'checked';?>> Aceitável
			&nbsp;&nbsp;&nbsp;
			<input type="radio" name="avdtempo" value="T" <?if($avdtempo == 'T') echo 'checked';?>> Teria que ser mais rápido
		</td>
	</tr>		
	<tr>
		<td class="subtitulodireita">3. Sobre o técnico,<br> como foi o tratamento e atenção dispensados durante o atendimento,<br> sua postura, o nível do conhecimento sobre o assunto,<br> o interesse em resolver o problema?</td>
		<td >
			<?
			$opcoes = array
				(
					"Ruim" => array
					(
							"valor" => "1",
							"id"    => "Truim"	
					),
					"Regular" => array
					(
							"valor" => "2",
							"id"    => "Tregular"	
					),
					"Bom" => array
					(
							"valor" => "3",
							"id"    => "Tbom"	
					),
					"Ótimo" => array
					(
							"valor" => "4",
							"id"    => "Totimo"	
					)					
				);
			campo_radio( 'avdtecnico', $opcoes, 'h' );	
		?>	
		</td>
	</tr>		

	<tr>
		<td class="subtitulodireita">4. No geral o atendimento foi:</td>
		<td>
			<?
			$opcoes = array
				(
					"Ruim" => array
					(
							"valor" => "1",
							"id"    => "Gruim"	
					),
					"Regular" => array
					(
							"valor" => "2",
							"id"    => "Gregular"	
					),
					"Bom" => array
					(
							"valor" => "3",
							"id"    => "Gbom"	
					),
					"Ótimo" => array
					(
							"valor" => "4",
							"id"    => "Gotimo"	
					)					
				);
			campo_radio( 'avdgeral', $opcoes, 'h' );	
		?>	
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Observação:</td>
		<td >
			<?=campo_textarea('avsobs', 'N ', $habil, '', 80, 5, 1000); ?>
		</td>
	</tr>
         <?php
            /*
            * Verifique se o estado atual está ?Aguardando validação?
            */
            $retturnEstadoAtual = wf_pegarEstadoAtual($docid);
            //if ($retturnEstadoAtual["esdid"] == "278" or $retturnEstadoAtual["esdid"] == "93"){
         ?>
        <?php
        /*
            * Quando a demanda for aberta como Suporte de atendimento, o sistema apresentará a aba "Avaliação da Demanda", 
            * será apresentado o texto acima do primeiro item de avaliação com o texto 
            * "Você esta avaliando o serviço do técnico ?Nome do técnico?.
            */
        $sqlOd = "SELECT
                        od.ordid 
                    FROM 
                        DEMANDAS.DEMANDA d
                        LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid 
                        LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
                    where d.dmdid = '{$dmdid}' ";
                    $retornoOd= $db->pegaLinha($sqlOd);   

        /*
         * Será apresentado para as demandas que receberem avaliações igual a ?Regular? ou ?Ruim? no ítem 3 e/ou 4, o sistema apresentará para o perfil de ?Administrador? 
         * o campo "Anular a negatividade da avaliação" do tipo um checkbox (que não virá preenchido automaticamente), 
         * onde permitirá que o administrador reavalie a avaliação feita pelo solicitante, 
         * o sistema não apagará a avaliação realizada anteriormente. 
         */
        ?>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<td>
	    	<input type='submit' class='botao' id="Salvar" value='Salvar' name='cadastrar' <?= $habil == 'N' ? 'disabled="disabled"' : ''?> />&nbsp;
	    	<!-- <input type='button' class='botao' value='Fechar' name='fechar' onclick='window.close();' />	-->    	
		</td>			
	</tr>
	
	<tr>
		<td colspan="2" bgcolor="#f5f5f5" height="30" ><b>Suas avaliações:</b></td>
	</tr>
	
</table>
<?php if ( in_array(DEMANDA_PERFIL_ADMINISTRADOR,$perfilUserAv) and $retornoOd['ordid'] == "3") {?>
    <?php
        $insereResposta = 0;
        if( in_array(DEMANDA_PERFIL_TECNICO, $perfilUserAv ) || in_array(DEMANDA_PERFIL_TECNICO1, $perfilUserAv ) || in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUserAv) ){
                $insereResposta = 1;
        }


        $sql = "SELECT '<center>' || to_char(a.avddata::timestamp,'DD/MM/YYYY HH24:MI') || '</center>', 
                                   (CASE a.avdprobres
                                                WHEN 'S' THEN '<center>SIM</center>'
                                                WHEN 'N' THEN '<center>NÃO</center>'
                                        END) AS avdprobres,
                                        (CASE a.avdtempo
                                                WHEN 'M' THEN '<center>MUITO BOM</center>'
                                                WHEN 'B' THEN '<center>BOM</center>'
                                                WHEN 'A' THEN '<center>ACEITÁVEL</center>'
                                                WHEN 'T' THEN '<center>TERIA QUE SER MAIS RÁPIDO</center>'
                                        END) AS avdtempo,
                                        (CASE a.avdtecnico
                                                WHEN '1' THEN '<center>RUIM</center>'
                                                WHEN '2' THEN '<center>REGULAR</center>'
                                                WHEN '3' THEN '<center>BOM</center>'
                                                WHEN '4' THEN '<center>ÓTIMO</center>'
                                        END) AS avdtecnico,
                                        (CASE a.avdgeral
                                                WHEN '1' THEN '<center>RUIM</center>'
                                                WHEN '2' THEN '<center>REGULAR</center>'
                                                WHEN '3' THEN '<center>BOM</center>'
                                                WHEN '4' THEN '<center>ÓTIMO</center>'
                                        END) AS avdgeral,
                                        avsobs,
                                        (CASE WHEN 1 = $insereResposta THEN
                                                        CASE WHEN a.avdresposta IS NULL THEN
                                                                        '<center>Resposta:<br><textarea rows=5 cols=30 name=\'resp'||a.avdid||'\' id=\'resp'||a.avdid||'\'></textarea><br><input type=\'button\' name=Salvar value=Salvar onclick=\'salvaRes('||a.avdid||');\'></center>'
                                                                ELSE
                                                                        '<center>Resposta:<br><textarea rows=5 cols=30 name=\'resp'||a.avdid||'\' id=\'resp'||a.avdid||'\'>'||a.avdresposta||'</textarea><br>De: '||u.usunome||'<br>Data: '||to_char(a.avddtresposta::timestamp,'DD/MM/YYYY HH24:MI')||'<br><input type=\'button\' name=Salvar value=Salvar onclick=\'salvaRes('||a.avdid||');\'></center>'
                                                        END 
                                                ELSE
                                                        a.avdresposta
                                        END) AS resposta,
                                        case when a2.ultima_data = a.avddata then
							case when a.avdtecnico in('1','2') then
								case when a.avdgeral in('1','2') then
                                                                    case when a.avnegatividade = '1' then
										'Deseja Anular: <input checked=\"checked\" class=\"checkbox\" type=\"checkbox\" id=\"avnegatividade\" name=\"avnegatividade\" value=\"1\">'
									else
										'Deseja Anular: <input class=\"checkbox\" type=\"checkbox\" id=\"avnegatividade\" name=\"avnegatividade\" value=\"1\">'	
									end
                                                                    
                                                                end
							end
		 				end as checkbox

                        FROM demandas.avaliacaodemanda a
                        left join (select MAX(a1.avddata) as ultima_data , a1.dmdid
 						 from demandas.avaliacaodemanda a1  
 							where a1.dmdid = {$dmdid}
 							group by a1.dmdid
 							 ) a2 on a2.dmdid = a.dmdid 
				left join seguranca.usuario u on u.usucpf = a.usucpftecnico
                        where a.avdstatus='A' and a.dmdid = {$dmdid}
                        order by 1 desc ";
                       // echo $sql;
        $cabecalho = array( "Data Inclusão","Item 1" , "Item 2", "Item 3", "Item 4", "Observação", "Resposta do Técnico", "Negatividade da Avaliação");
        $db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '');
    ?>
<?php } else {?>
    <?php
        $insereResposta = 0;
        if( in_array(DEMANDA_PERFIL_TECNICO, $perfilUserAv ) || in_array(DEMANDA_PERFIL_TECNICO1, $perfilUserAv ) || in_array(DEMANDA_PERFIL_SUPERUSUARIO,$perfilUserAv) ){
                $insereResposta = 1;
        }


        $sql = "SELECT '<center>' || to_char(avddata::timestamp,'DD/MM/YYYY HH24:MI') || '</center>', 
                                   (CASE avdprobres
                                                WHEN 'S' THEN '<center>SIM</center>'
                                                WHEN 'N' THEN '<center>NÃO</center>'
                                        END) AS avdprobres,
                                        (CASE avdtempo
                                                WHEN 'M' THEN '<center>MUITO BOM</center>'
                                                WHEN 'B' THEN '<center>BOM</center>'
                                                WHEN 'A' THEN '<center>ACEITÁVEL</center>'
                                                WHEN 'T' THEN '<center>TERIA QUE SER MAIS RÁPIDO</center>'
                                        END) AS avdtempo,
                                        (CASE avdtecnico
                                                WHEN '1' THEN '<center>RUIM</center>'
                                                WHEN '2' THEN '<center>REGULAR</center>'
                                                WHEN '3' THEN '<center>BOM</center>'
                                                WHEN '4' THEN '<center>ÓTIMO</center>'
                                        END) AS avdtecnico,
                                        (CASE avdgeral
                                                WHEN '1' THEN '<center>RUIM</center>'
                                                WHEN '2' THEN '<center>REGULAR</center>'
                                                WHEN '3' THEN '<center>BOM</center>'
                                                WHEN '4' THEN '<center>ÓTIMO</center>'
                                        END) AS avdgeral,
                                        avsobs,
                                        (CASE WHEN 1 = $insereResposta THEN
                                                        CASE WHEN avdresposta IS NULL THEN
                                                                        '<center>Resposta:<br><textarea rows=5 cols=30 name=\'resp'||avdid||'\' id=\'resp'||avdid||'\'></textarea><br><input type=\'button\' name=Salvar value=Salvar onclick=\'salvaRes('||avdid||');\'></center>'
                                                                ELSE
                                                                        '<center>Resposta:<br><textarea rows=5 cols=30 name=\'resp'||avdid||'\' id=\'resp'||avdid||'\'>'||avdresposta||'</textarea><br>De: '||u.usunome||'<br>Data: '||to_char(a.avddtresposta::timestamp,'DD/MM/YYYY HH24:MI')||'<br><input type=\'button\' name=Salvar value=Salvar onclick=\'salvaRes('||avdid||');\'></center>'
                                                        END 
                                                ELSE
                                                        avdresposta
                                        END) AS resposta
                        FROM demandas.avaliacaodemanda a
                        left join seguranca.usuario u on u.usucpf = a.usucpftecnico
                        where avdstatus='A' and dmdid = {$dmdid}
                        order by avddata desc ";
        $cabecalho = array( "Data Inclusão","Item 1" , "Item 2", "Item 3", "Item 4", "Observação", "Resposta do Técnico");
        $db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '');
    ?>
<?php } ?>
</form>
</body>

<script type="text/javascript">
        jQuery(document).ready(function(){
             $("#avnegatividade").click(function() {
                var nNhecked;
                
                if($("input#avnegatividade:checked").length === 1){
                    nNhecked = "trueChecked";
                }else{
                    nNhecked = "falseChecked";
                }
                
                $.ajax({
   		type: "POST",
   		url: "demandas.php?modulo=principal/avaliacao&acao=A",
   		data: "UpdateNegatividade="+nNhecked,
                    success: function(msg){
                        if(msg == 'OK'){
                            alert("Operação efetuda com sucesso.");
                        }
                    }
 		});
             });
            
            
        });
    
	function salvaRes(avdid){
		resp = document.getElementById('resp'+avdid);
		
		if(resp.value == ''){
			alert ('Campo Resposta é obrigatório');
			resp.focus();
			return false;
		}
		
		$.ajax({
   		type: "POST",
   		url: "demandas.php?modulo=principal/avaliacao&acao=A",
   		data: "avdidAjax="+avdid+"&resposta="+resp.value,
                    success: function(msg){
                        if(msg == 'OK'){
                        alert("Operação efetuda com sucesso.");
                        }
                    }
 		});

	}	
</script>
