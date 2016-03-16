<?php

$ntcid = $_REQUEST['ntcid'];

	$sql = "SELECT arqid FROM academico.notatecnica WHERE ntcid = '$ntcid'";
	$arqid = $db->pegaUm($sql);
	if($arqid){
		$sql ="SELECT * FROM public.arquivo WHERE arqid = '$arqid'";
		        $arquivo = $db->carregar($sql);
		        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		        //$caminho = APPRAIZ."/arquivos/sig/".$ntcid;
		        
		        if ( !is_file( $caminho ) ) {
		            echo "<script>alert('Arquivo não encontrado!');window.close();</script>";
		            exit();
		        }
		        else{
				    header("Pragma: public");
				    header("Expires: 0");
				    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
				    header("Cache-Control: public");
				    header("Content-Description: File Transfer");
				    header("Content-Transfer-Encoding: binary");
    				
			        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
			        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
			        
			        //header( 'Content-Disposition: attachment; filename='.$filename); //Forçar download
			        readfile( $caminho );
			        exit();
		        }
		        
	}
	else{
		echo "<script>alert('Arquivo não encontrado!');window.close();</script>";
	}
?>