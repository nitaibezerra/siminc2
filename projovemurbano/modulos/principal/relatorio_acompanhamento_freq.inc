<?php 
// ver($_POST,d);
function fazerDownload(){
	unset($_REQUEST['requisicao']);
	$arqid = $_REQUEST['arqid'];
	ob_get_clean();
	require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec(null,null,'projovemurbano');
	$file->getDownloadArquivo($arqid);
	die;
} 

if($_REQUEST['requisicao']){
    $_REQUEST['requisicao']();
    die;
}   

if (isset($_REQUEST['buscar'])) {
    include $_REQUEST['relatorio'];
    exit;
}

monta_titulo('Acompanhamento de Frequência e Notas','');
$bloq = 'S';
$perfis = pegaPerfilGeral();

if(in_array(PFL_SUPER_USUARIO, $perfis)
	|| in_array(PFL_DIRETOR_NUCLEO, $perfis)){
	$bloqueio = 'S';
}else{
	$bloqueio = 'N';
}

	
if($_SESSION['projovemurbano']['ppuid'] == 2 ||$_SESSION['projovemurbano']['ppuid'] == 3){
	$filtrotprid = "AND tprid is not null";
	if($_SESSION['projovemurbano']['ppuid'] == 3){
		$filtrotprid = "AND tprid='".$_SESSION['projovemurbano']['tprid']."'";
	}
}else{
	$filtrotprid = "AND tprid is  null";
}    
     
	$sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pmustatus='A' $filtrotprid";
	$possuipolo = $db->pegaUm($sql);
//     $where = array();
    
extract($_REQUEST);
    
$perfis = pegaPerfilGeral();
    
if(!$db->testa_superuser()||!in_array(PFL_EQUIPE_MEC, $perfis)||!in_array(PFL_ADMINISTRADOR, $perfis)) { 
        
	$perfis = pegaPerfilGeral();
        
	if(in_array(PFL_DIRETOR_POLO, $perfis)) {
		$inner_perfil = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.polid=cae.polid)";
	}
        
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
		$inner_perfil = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND (ur.entid=nes.entid)";
	}
}

if($_SESSION['projovemurbano']['ppuid'] ){
	$tprid = 0;
	if($_REQUEST['turid']){ 
		if($_SESSION['projovemurbano']['ppuid']==2){
			$sqlprograma="SELECT
							tprid
						FROM
							projovemurbano.nucleo
						WHERE
							ppuid = {$_SESSION['projovemurbano']['ppuid']}
						AND nucid = $nucid";
			$tprid = $db->pegaUm($sqlprograma);
		}elseif($_SESSION['projovemurbano']['ppuid']==3){
			$tprid = $_SESSION['projovemurbano']['tprid'];
		}
		if($tprid==2){
			$presencaminima =972;
		}else{
			$presencaminima =1080;
		}
		
		if($situacao == 'A'){
			$filtrosituacao =" HAVING(((SUM(total) >= $presencaminima) AND (notatotal >= 1100)) or (( presencaconselho>= $presencaminima) AND (notaconselho >= 1100)))";
		}elseif($situacao == 'R'){
			$filtrosituacao =" HAVING(((SUM(total) < $presencaminima) OR (notatotal < 1100)) AND(notaconselho is NULL))";
		}
	    
		if( $turid != '' ){
			$where = "cae.turid = $turid";
			$whereO = "tra.turid_origem = $turid AND";
	// 		$whereD = "cae.turid = $turid AND";
		}
	
		$sql = "SELECT  caeid,
						nome,
						cpf,
						sum(qtd_periodo1) as qtd_periodo1,
						sum(qtd_periodo2) as qtd_periodo2,
						sum(qtd_periodo3) as qtd_periodo3,
						sum(qtd_periodo4) as qtd_periodo4,
						sum(qtd_periodo5) as qtd_periodo5,
						sum(qtd_periodo6) as qtd_periodo6,
						sum(qtd_periodo7) as qtd_periodo7,
						sum(qtd_periodo8) as qtd_periodo8,
						sum(qtd_periodo9) as qtd_periodo9,
						sum(qtd_periodo10) as qtd_periodo10,
						sum(qtd_periodo11) as qtd_periodo11,
						sum(qtd_periodo12) as qtd_periodo12,
						sum(qtd_periodo13) as qtd_periodo13,
						sum(qtd_periodo14) as qtd_periodo14,
						sum(qtd_periodo15) as qtd_periodo15,
						sum(qtd_periodo16) as qtd_periodo16,
						sum(qtd_periodo17) as qtd_periodo17,
						sum(qtd_periodo18) as qtd_periodo18,
						SUM(total) as total,
						nota1,
						nota2,
						nota3,
						notatotal,
						presencaconselho,
						notaconselho         
					FROM 
						(
						SELECT 
							cae.caeid as caeid,
							cae.caenome as nome,
							cae.caecpf as cpf,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 1 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo1,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 2 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo2,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 3 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo3,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 4 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo4,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 5 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo5,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 6 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo6,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 7 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo7,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 8 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo8,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 9 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo9,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 10 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo10,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 11 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo11,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 12 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo12,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 13 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo13,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 14 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo14,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 15 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo15,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 16 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo16,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 17 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo17,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 18 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo18,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae.caeid),0)as total,
							coalesce(npc.notaciclo1,0) as nota1,
							coalesce(npc.notaciclo2,0) as nota2,
							coalesce(npc.notaciclo3,0) as nota3,
							(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal,
							totalnotas as notaconselho,
							totalpresenca as presencaconselho
						FROM  projovemurbano.cadastroestudante cae
						LEFT JOIN projovemurbano.avaliacaoconselhodeclasse avc ON avc.caeid = cae.caeid   
						INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae.caeid AND frq.frqstatus = 'A'
						INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
						INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
						INNER JOIN projovemurbano.periodocurso prc USING(perid)
						LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae.caeid AND npc.npc_status = 'A' AND npc.turid = $turid
						WHERE  
							
						$where AND cae.caestatus = 'A'
	                        
						GROUP BY 
							cae.caeid,
							cae.caenome,
							cae.caeid,
							dia.turid,
							cae.caecpf,
							prc.perid,
							prc.perperiodo,
							frq.caeid,
							cae.turid,
							npc.notaciclo1,
							npc.notaciclo2,
							npc.notaciclo3,
							notatotal,
							totalnotas,
							totalpresenca) as somatoria
					GROUP BY
						caeid, 
						nome,
						cpf,
						nota1,
						nota2,
						nota3,
						notatotal,
						notaconselho,
						presencaconselho
					$filtrosituacao
					ORDER BY nome";
// 		ver($sql,d);
	        $dados2 = $db->carregar($sql);
	        
	        $caeid[] = $dados2['caeid'];
	
			$sql2="SELECT  
						nome,
						cpf,
						turid,
						sum(qtd_periodo1) as qtd_periodo1,
						sum(qtd_periodo2) as qtd_periodo2,
						sum(qtd_periodo3) as qtd_periodo3,
						sum(qtd_periodo4) as qtd_periodo4,
						sum(qtd_periodo5) as qtd_periodo5,
						sum(qtd_periodo6) as qtd_periodo6,
						sum(qtd_periodo7) as qtd_periodo7,
						sum(qtd_periodo8) as qtd_periodo8,
						sum(qtd_periodo9) as qtd_periodo9,
						sum(qtd_periodo10) as qtd_periodo10,
						sum(qtd_periodo11) as qtd_periodo11,
						sum(qtd_periodo12) as qtd_periodo12,
						sum(qtd_periodo13) as qtd_periodo13,
						sum(qtd_periodo14) as qtd_periodo14,
						sum(qtd_periodo15) as qtd_periodo15,
						sum(qtd_periodo16) as qtd_periodo16,
						sum(qtd_periodo17) as qtd_periodo17,
						sum(qtd_periodo18) as qtd_periodo18,
						SUM(total) as total,
						nota1,
						nota2,
						nota3,
						notatotal         
					FROM 
						(
						SELECT 
							cae.caenome as nome,
							cae.caecpf as cpf,
							cae.turid as turid,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 1 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo1,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 2 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo2,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 3 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo3,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 4 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo4,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 5 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo5,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 6 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo6,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 7 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo7,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 8 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo8,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 9 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo9,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 10 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo10,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 11 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo11,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 12 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo12,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 13 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo13,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 14 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo14,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 15 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo15,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 16 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo16,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 17 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo17,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE prc.perid = (SELECT perid FROM projovemurbano.periodocurso prc WHERE prc.perperiodo = 18 AND prc.ppuid = {$_SESSION['projovemurbano']['ppuid']})),0) as qtd_periodo18,
							coalesce((SELECT SUM(frq.frqqtdpresenca) WHERE frq.caeid = cae.caeid),0)as total,
							coalesce(npc.notaciclo1,0) as nota1,
							coalesce(npc.notaciclo2,0) as nota2,
							coalesce(npc.notaciclo3,0) as nota3,
							(coalesce(npc.notaciclo1,0) + coalesce(npc.notaciclo2,0) + coalesce(npc.notaciclo3,0))as notatotal
						FROM projovemurbano.cadastroestudante cae
						INNER JOIN projovemurbano.transferencia       tra    ON cae.caeid = tra.cad_caeid  
						LEFT JOIN projovemurbano.historico_transferencia hst ON hst.traid = tra.traid AND hst.htrid = tra.htrid_ultimo
						INNER JOIN projovemurbano.frequenciaestudante frq ON frq.caeid = cae.caeid  AND frq.frqstatus = 'I'
						INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
						INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid 
						INNER JOIN projovemurbano.periodocurso prc USING(perid)
						LEFT  JOIN projovemurbano.notasporciclo npc ON npc.caeid = cae.caeid AND npc.npc_status = 'I'
						WHERE  
	                           $whereO hst.shtid_status = 3 AND cae.caestatus = 'A'
						GROUP BY 
							cae.caenome,
							cae.caecpf,
							prc.perid,
							prc.perperiodo,
							frq.caeid,
							cae.caeid,
							dia.turid,
							cae.turid,
							npc.notaciclo1,
							npc.notaciclo2,
							npc.notaciclo3,
							notatotal) as somatoria
					GROUP BY 
						nome,
						cpf,
						turid,
						nota1,
						nota2,
						nota3,
						notatotal
					ORDER BY nome";
//                ver($sql2,d);           
			$dados = $db->carregar($sql2);
//                ver(d);
			
    		$sqlcoord =  "
        		SELECT DISTINCT                
		            ( coalesce(est.estuf,mun.estuf||' - '||mun.mundescricao) ) as localorigem,
		            'NUCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||
		            COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as nucleo,
		            tur.turdesc
        		FROM 
            		projovemurbano.cadastroestudante cae
        		LEFT JOIN projovemurbano.polo pol
            	LEFT JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid ON pol.polid  = cae.polid
        		LEFT  JOIN projovemurbano.nucleo                        nuc  
            	LEFT JOIN projovemurbano.municipio                 muni  ON muni.munid = nuc.munid
            	LEFT JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = muni.pmuid ON nuc.nucid  = cae.nucid
		        LEFT  JOIN projovemurbano.turma                 tur ON tur.nucid = nuc.nucid            
		        LEFT  JOIN projovemurbano.projovemurbano                pju ON pju.pjuid = pmu.pjuid OR pju.pjuid = pmu2.pjuid
		        LEFT  JOIN territorios.municipio                        mun ON mun.muncod = pju.muncod
		        LEFT  JOIN territorios.estado                           est ON est.estuf = pju.estuf
        		WHERE
            		tur.turid  = $turid";
//    ver($sqlcoord,d);
    		$coord = $db->pegaLinha($sqlcoord);
//     ver($coord);
//     ver($_POST,$_REQUEST,d);
    		$carregaarquivo = 
								"SELECT 
									CASE WHEN art.arqid IS NULL
		                    		THEN ''
		                   			ELSE '<a id='||art.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>'|| arqnome ||'</a>'
		                			END as baixararq,
		                			art.arqid
					   			FROM
						   			projovemurbano.arquivo_turma art
					   			INNER JOIN   public.arquivo arq ON arq.arqid = art.arqid
					   			WHERE
									turid = $turid AND arqstatus='A'";
    		
    		$arquivo = $db->pegaLinha($carregaarquivo);
    	}
    ?>
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script language="javascript" type="text/javascript">
    jQuery(document).ready(function() {

    	jQuery('.salvar').click(function(){
    		
			var valorx = false;

			if(jQuery('[id="caeid"]').length ){			
					valorx = true;
			}
	// 							
				if( !valorx ){
					alert('Preencha a Nota/Presença de ao menos um dos alunos');
					return false;
				}
			
    		jQuery('#requisicao').val('salvar_dados_conselho');
    		jQuery('#form').submit();
    			
    	});

    	$('#btnImprimir').click(function(){
            window.print();
        });
        
		jQuery('#marcar-todos').live('click',function(){
			jQuery('.doc').attr('checked', jQuery(this).attr('checked'));
		});
        
    	jQuery('#nucid').addClass('obrigatorio');
    	jQuery('#turid').addClass('obrigatorio');
        jQuery('.pesquisar').click(function(){
        	var erro = false;
    		jQuery('.obrigatorio').each(function(){
    			if( jQuery(this).val() == '' ){
    				alert('Campo obrigatório.');
    				jQuery(this).focus();
    				erro = true;
    				return false;
    			}
    		});
    		if( erro ){
    			return false;
    		}
            jQuery('#polid').attr('selected',true);
            jQuery('#nucid').attr('selected',true);
            jQuery('#turid').attr('selected',true);
            jQuery('#requisicao').val('');
            jQuery('#form').submit();
        });

        jQuery('.baixar').click(function(){
            jQuery('#arqid').val( jQuery(this).attr('id') );
            jQuery('#form').submit();
        });
    });
    
    function filtraNucleoCombo(polid) {
    	jQuery('#td_nucleo').html('Carregando...');
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
 			data: "requisicao=buscarComboNucleo&polid=" + polid + "&bloq=<?=$bloq ?>",
			async: false,
			success: function(msg) {
    	        document.getElementById('td_nucleo').innerHTML = msg;
			}
		});
	}
    
    function buscarTurmas(nucid) {
	  	if (nucid != '') {
	    	jQuery('#td_turma').html('Carregando...');
    		jQuery.ajax({
	      		type: "POST",
	      		url: window.location.href,
	      		data: "requisicao=buscarComboTurma&nucid=" + nucid + "&bloq=<?=$bloq ?>",
	      		async: false,
	      		success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
	   	 	});
	  	}
	}
    
    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo( campo )
    {
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
    
    </script>
<style>
.div_muda {
	align: top;
}
</style>
<div id="dialog-detalhe"></div>
<form id="form" enctype="multipart/form-data" name="form" method="POST">
	<input type="hidden" value="" name="requisicao" id="requisicao" /> 
	<input type="hidden" name="arqid" id="arqid" /> 
	<input type="hidden" value="" name="caeid" id="caeid" />
<?if( $_SESSION['projovemurbano']['ppuano'] && $_SESSION['projovemurbano']['ppuid'] ){?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
		cellPadding="3" align="Center" border="0">
		<tr>
			<td class="SubTituloCentro" width="50%" colspan="2">Filtros</td>
		</tr>
		<tr>
			<td colspan="2">
				<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
					cellPadding="3" align="Center" border="0">
                        <?if($possuipolo == 't'){?>
                         <tr>
						<td class="SubTituloDireita" width="15%">Polo:</td>
						<td><?php buscarComboPolo(); ?></td>
					</tr>                     
                        <?php }?>
                         	<tr>
						<td class="SubTituloDireita" width="15%">Núcleo:</td>
						<td id="td_nucleo"><?php  buscarComboNucleo(); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Turma:</td>
						<td id="td_turma"><?php buscarComboTurma(); ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Situação:</td>
						<td id="td_turma">
	                          		<?php $arrSituacao = array( 
								 			array("codigo" => "A", "descricao" => "Aprovados"), 
								 			array("codigo" => "R", "descricao" => "Reprovados")
							 			); 
									$db->monta_combo('situacao', $arrSituacao, 'S', 'Todas', '', '', '', '150', 'N', 'situacao');  ?>
								</td>
					</tr>
				</table>
			</td>
		
		
		<tr>
			<td class="SubTituloCentro" colspan="4"><input type="button"
				class="pesquisar" value="Pesquisar"></td>
		</tr>
    <?if($_REQUEST['turid']){?>           
            <tr>
			<td>
				<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
					cellPadding="3" align="Center" border="0">
					<tr>
						<td rowspan="3" style="width: 150px;"><img
							src="../imagens/projovemurbano_cinza.jpg" alt="PROJOVEM URBANO"
							title="PROJOVEMURBANO" /></td>
						<td class="SubTituloDireita" width="15%">Nome Coordenação:</td>
						<td><?=$coord['localorigem']?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Núcleo/Escola:</td>
						<td><?=$coord['nucleo']?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Turma:</td>
						<td><?=$coord['turdesc']?></td>
					</tr>
				</table>
			</td>
		</tr>
		<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="5">
			<tr>
				<td class="SubTituloCentro" rowspan="3" width="24%">ESTUDANTE</td>
				<td class="SubTituloCentro" rowspan="3" width="7%">CPF</td>
				<td class="SubTituloCentro" colspan="18">CARGA HORÁRIA CURSADA</td>
				<td class="SubTituloCentro" rowspan="3" width="4%">CARGA HORÁRIA
					TOTAL</td>
				<td class="SubTituloCentro" colspan="3">PONTUAÇÃO DAS <br>AVALIAÇÕES
					FORMATIVAS
				</td>
				<td class="SubTituloCentro" rowspan="3" width="4%">PONTUAÇÃO TOTAL</td>
				<td class="SubTituloCentro" rowspan="3" width="4%">RESULTADO FINAL</td>
                    <?php if($arquivo['baixararq']!=''){?>
                    	<td class="SubTituloCentro" rowspan="2" colspan="2"
					width="4%">RESULTADO APÓS O CONSELHO DE CLASSE</td>
                    <?php }?>
                </tr>
			<tr>
				<td colspan="3" class="SubTituloCentro">UF I</td>
				<td colspan="3" class="SubTituloCentro">UF II</td>
				<td colspan="3" class="SubTituloCentro">UF III</td>
				<td colspan="3" class="SubTituloCentro">UF IV</td>
				<td colspan="3" class="SubTituloCentro">UF V</td>
				<td colspan="3" class="SubTituloCentro">UF VI</td>
				<td colspan="3" class="SubTituloCentro">CICLOS</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" width="2%">1</td>
				<td class="SubTituloCentro" width="2%">2</td>
				<td class="SubTituloCentro" width="2%">3</td>
				<td class="SubTituloCentro" width="2%">4</td>
				<td class="SubTituloCentro" width="2%">5</td>
				<td class="SubTituloCentro" width="2%">6</td>
				<td class="SubTituloCentro" width="2%">7</td>
				<td class="SubTituloCentro" width="2%">8</td>
				<td class="SubTituloCentro" width="2%">9</td>
				<td class="SubTituloCentro" width="2%">10</td>
				<td class="SubTituloCentro" width="2%">11</td>
				<td class="SubTituloCentro" width="2%">12</td>
				<td class="SubTituloCentro" width="2%">13</td>
				<td class="SubTituloCentro" width="2%">14</td>
				<td class="SubTituloCentro" width="2%">15</td>
				<td class="SubTituloCentro" width="2%">16</td>
				<td class="SubTituloCentro" width="2%">17</td>
				<td class="SubTituloCentro" width="2%">18</td>
				<td class="SubTituloCentro" width="3%">1º</td>
				<td class="SubTituloCentro" width="3%">2º</td>
				<td class="SubTituloCentro" width="3%">3º</td>
                    <?php if($arquivo['baixararq']!=''){?>
                    <td class="SubTituloCentro" width="3%"><input
					type="checkbox" id="marcar-todos"></td>
                    <?php }?>
                </tr>
        <?
        $totalalunosTrans = count($dados2);
        if( is_array($dados2) ){
		$totalalunosTrans = count($dados2);
            foreach( $dados2 as $dado2){
				$sqlchecaalunotrans="
									SELECT 
										traperiodotransferido 
									FROM
										projovemurbano.transferencia       tra    
									INNER JOIN projovemurbano.historico_transferencia hst ON hst.traid = tra.traid AND hst.htrid = tra.htrid_ultimo AND hst.shtid_status = 3
									WHERE
										tra.cad_caeid = {$dado2['caeid']}";
			 	$checaalunotrans = $db->pegaUm($sqlchecaalunotrans);
	 			
			 	$sqlchecaconselho = "SELECT 
										'Aprovado'
									FROM 
										projovemurbano.avaliacaoconselhodeclasse 
									WHERE 
										turid = $turid   
									AND caeid = {$dado2['caeid']}
									AND totalpresenca = $presencaminima 
									AND totalnotas = 1100";
			 	$checaconselho = $db->pegaUm($sqlchecaconselho);
        ?>
            <tr>
				<td><?=$dado2['nome']?></td>
				<td align="center"><?=$dado2['cpf']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '1'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo1']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '2'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo2']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '3'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo3']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '4'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo4']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '5'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo5']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '6'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo6']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '7'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo7']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '8'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo8']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '9'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo9']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '10'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo10']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '11'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo11']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '12'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo12']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '13'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo13']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '14'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo14']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '15'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo15']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '16'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo16']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '17'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo17']?></td>
				<td align="center" style="background-color:<?=($checaalunotrans?($checaalunotrans >= '18'?"#FF4500":""):"")?>"><?=$dado2['qtd_periodo18']?></td>
				<td align="center"><?=$dado2['total']?></td>
				<td align="center"><?=$dado2['nota1']?></td>
				<td align="center"><?=$dado2['nota2']?></td>
				<td align="center"><?=$dado2['nota3']?></td>
				<td align="center"><?=$dado2['notatotal']?></td>
				<td align="center">
                <?  if($dado2['notatotal']>=1100 && $dado2['total']>= $presencaminima){?>
						Aprovado
				<?php }else{ ?>	
						 Reprovado
				<?php }?>	  
                </td>
                <?if(!in_array(PFL_CONSULTA, $perfis)){?> 
	                <?php if($arquivo['baixararq']!=''){?>
	                    <td align="center">
	                    <?  if($dado2['notatotal']>=1100 && $dado2['total']>= $presencaminima){?>
	                    --
						<?	}else{ 
								if($checaconselho){	?>
								 <?=$checaconselho ?>
							<?  }else{ ?> 
								 	<input type="checkbox" class="doc" name="caeid[]"
						value="<?=$dado2['caeid']?>" />
								<? } ?>
						<?	}?>
	                    </td>
	                <?php }?>
                <?}?>
            </tr>
        <?
            }
        ?>    
        <tr>
				<td colspan="27" class="SubTituloEsquerda">Total de Alunos Ativos:<?=$totalalunosTrans?></td>
			</tr>
        <?    
        }
        ?>
         <?
        if( is_array($dados) ){
			$totalalunos = count($dados);
            foreach( $dados as $dado){
        ?>
            <tr>
				<td><?=$dado['nome']?></td>
				<td align="center"><?=$dado['cpf']?></td>
				<td align="center"><?=$dado['qtd_periodo1']?></td>
				<td align="center"><?=$dado['qtd_periodo2']?></td>
				<td align="center"><?=$dado['qtd_periodo3']?></td>
				<td align="center"><?=$dado['qtd_periodo4']?></td>
				<td align="center"><?=$dado['qtd_periodo5']?></td>
				<td align="center"><?=$dado['qtd_periodo6']?></td>
				<td align="center"><?=$dado['qtd_periodo7']?></td>
				<td align="center"><?=$dado['qtd_periodo8']?></td>
				<td align="center"><?=$dado['qtd_periodo9']?></td>
				<td align="center"><?=$dado['qtd_periodo10']?></td>
				<td align="center"><?=$dado['qtd_periodo11']?></td>
				<td align="center"><?=$dado['qtd_periodo12']?></td>
				<td align="center"><?=$dado['qtd_periodo13']?></td>
				<td align="center"><?=$dado['qtd_periodo14']?></td>
				<td align="center"><?=$dado['qtd_periodo15']?></td>
				<td align="center"><?=$dado['qtd_periodo16']?></td>
				<td align="center"><?=$dado['qtd_periodo17']?></td>
				<td align="center"><?=$dado['qtd_periodo18']?></td>
				<td align="center"><?=$dado['total']?></td>
				<td align="center"><?=$dado['nota1']?></td>
				<td align="center"><?=$dado['nota2']?></td>
				<td align="center"><?=$dado['nota3']?></td>
				<td align="center"><?=$dado['notatotal']?></td>
			</tr>
    <?
            }
    ?>
    <tr>
				<td colspan="27" class="SubTituloEsquerda">Total de Alunos Transferidos:<?=$totalalunos?></td>
			</tr>
    	<?
        }
        ?>
        	<tr>
				<td class="SubTituloEsquerda">
					Para registrar o resultado do Conselho
					de Classe anexe a Ata assinada por todos os participantes do
					Conselho:</td>
				<td colspan="26">
					<?=$arquivo['baixararq']; ?>
					<input type="hidden" name="arqid2" id="arqid2" value="<?php echo $arquivo['arqid']; ?>" /> 
					<input type="file" name="arquivo" id="anexo" style="cursor: pointer">
                </td>
			</tr>
			<tr>
				<td colspan="27" class="SubTituloEsquerda">
					<?if($arquivo){?>
						Os alunos que foram aprovados pelo conselho de classe não atingiram durante o curso o valor mínimo para total de Frequência e/ou Notas. Portanto o conselho decidiu aprovar esses alunos completando os requisitos que faltaram, lançando quando necessário para a provação <?echo $presencaminima;?> horas na presença e/ou 1100 pontos de somatório das notas.
					<?}?>
                </td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="27">
	                <?if($db->testa_superuser() 
		                || in_array(PFL_SUPER_USUARIO, $perfis)
						|| in_array(PFL_DIRETOR_NUCLEO, $perfis)){?>
						<input type="button" class="salvar" value="Salvar">
					<?}?>
					<input type="button" name="btnFecharTrabalho" id="btnImprimir"
					value="Imprimir" />
				</td>
			</tr>
			<tr>
				<td colspan="27">
					<!-- Cabeçalho -->
					<table id="cabecalho" border="0"
						style="width: 100%; border: 1px solid black;">
						<tr>
							<td><strong>Data de Emissão: <?php echo date("d/m/Y - H:i:s") ?></strong></td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</table>
</form>
<?  
        }
    }
}?>